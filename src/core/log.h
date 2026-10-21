namespace core
{
    void log(const char * format, ...)
    {
        static char buffer[64];

        va_list args;

        va_start(args, format); // 1. Инициализируем указатель на аргументы

        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args); // 3. Освобождаем ресурсы

        Serial.println(buffer);
    }
}