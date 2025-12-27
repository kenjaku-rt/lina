# Обычный Julia файл
println("[Julia App] Читаю общую память...")
# lina_array уже доступен глобально
println("[Julia App] Данные: ", lina_array)

# Возводим в квадрат
lina_array .= lina_array .^ 2
println("[Julia App] Данные возведены в квадрат.")