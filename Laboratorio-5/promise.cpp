#include <iostream>
#include <thread>
#include <future>

// Función que calcula el cuadrado de un número
void calculateSquare(std::promise<int> promiseObj, int number) {
    // Simular un trabajo pesado
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int result = number * number;

    // Enviar el resultado al hilo principal
    promiseObj.set_value(result);
}

int main() {
    // Crear un objeto promise
    std::promise<int> promiseObj;

    // Obtener el future asociado al promise
    std::future<int> futureObj = promiseObj.get_future();

    int number = 5;

    // Crear un hilo para calcular el cuadrado
    std::thread workerThread(calculateSquare, std::move(promiseObj), number);

    // Mientras se calcula, podemos realizar otras tareas
    std::cout << "Calculando el cuadrado de " << number << "...\n";

    // Bloquear hasta que el hilo complete el cálculo y obtengamos el resultado
    int result = futureObj.get();

    // Mostrar el resultado
    std::cout << "El cuadrado de " << number << " es: " << result << std::endl;

    // Esperar a que el hilo termine
    workerThread.join();

    return 0;
}
