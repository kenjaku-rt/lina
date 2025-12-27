#include <Python.h>
#include <julia.h>
#include <stdio.h>

void run_python_file(const char* filename, PyObject* mem_view) {
    PyObject *main_module = PyImport_AddModule("__main__");
    PyObject *global_dict = PyModule_GetDict(main_module);
    
    // Внедряем объект в пространство Python перед запуском файла
    PyDict_SetItemString(global_dict, "lina_raw_mem", mem_view);
    
    FILE* fp = fopen(filename, "r");
    if (fp) {
        PyRun_SimpleFile(fp, filename);
        fclose(fp);
    }
}

void run_julia_file(const char* filename, double* data, int size) {
    // 1. Подготовка массива (как и было)
    jl_value_t* array_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
    jl_array_t* jl_array = jl_ptr_to_array_1d(array_type, data, size, 0);

    // 2. РЕШЕНИЕ: Явно объявляем имя в модуле Main
    // Мы создаем "binding" (привязку), если её еще нет
    jl_sym_t *var_name = jl_symbol("lina_array");
    jl_binding_t *b = jl_get_binding_wr(jl_main_module, var_name, 1); // 1 = создать, если нет
    
    // Теперь присваиваем значение этой привязке
    jl_checked_assignment(b, jl_main_module, var_name, (jl_value_t*)jl_array);

    // 3. Формируем команду include
    char code[256];
    snprintf(code, sizeof(code), "include(\"%s\")", filename);

    printf("[C -> Julia] Загружаю файл: %s\n", filename);
    jl_eval_string(code);
}

int main() {
    int size = 5;
    double *shared_mem = (double*)malloc(size * sizeof(double));
    for(int i=0; i<size; i++) shared_mem[i] = (double)i + 1.0;

    // --- Инициализация ---
    Py_Initialize();
    jl_init();

    // Создаем Python MemoryView
    PyObject *mem_view = PyMemoryView_FromMemory((char*)shared_mem, size * sizeof(double), PyBUF_WRITE);

    // --- Исполнение ---
    printf("--- Запуск Lina Bridge ---\n");
    
    run_python_file("app.py", mem_view);
    run_julia_file("app.jl", shared_mem, size);

    // --- Результат ---
    printf("[C Host] Итоговое состояние памяти: ");
    for(int i=0; i<size; i++) printf("%.1f ", shared_mem[i]);
    printf("\n");

    // --- Очистка ---
    Py_XDECREF(mem_view);
    Py_Finalize();
    jl_atexit_hook(0);
    free(shared_mem);
    return 0;
}
