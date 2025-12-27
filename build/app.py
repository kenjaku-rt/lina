# Обычный Python файл
def main():
    # Мы ожидаем, что lina_raw_mem уже в глобальных переменных
    view = lina_raw_mem.cast('d') 
    print(f"[Python App] Получены данные: {view.tolist()}")
    
    # Инвертируем значения для примера
    for i in range(len(view)):
        view[i] = view[i] * -1
    
    print("[Python App] Данные инвертированы.")

main()