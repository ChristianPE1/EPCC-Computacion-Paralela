#include <iostream>
#include <pthread.h>
#include <cstdlib>

using namespace std;

// Nodo
struct ListNode
{
    int data;
    ListNode *next;
};

// Estructura del Read-Write Lock
struct RWLock
{
    pthread_mutex_t mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    int readers = 0;
    int writers = 0;
    int pending_writers = 0;
};

// Funciones del Read-Write Lock
void rwlock_init(RWLock *lock)
{
    pthread_mutex_init(&lock->mutex, nullptr);
    pthread_cond_init(&lock->read_cond, nullptr);
    pthread_cond_init(&lock->write_cond, nullptr);
}

void rwlock_read_lock(RWLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    while (lock->writers > 0 || lock->pending_writers > 0)
    {
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }
    lock->readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void rwlock_read_unlock(RWLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->readers--;
    if (lock->readers == 0)
    {
        pthread_cond_signal(&lock->write_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void rwlock_write_lock(RWLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->pending_writers++;
    while (lock->writers > 0 || lock->readers > 0)
    {
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }
    lock->pending_writers--;
    lock->writers++;
    pthread_mutex_unlock(&lock->mutex);
}

void rwlock_write_unlock(RWLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->writers--;
    if (lock->pending_writers > 0)
    {
        pthread_cond_signal(&lock->write_cond);
    }
    else
    {
        pthread_cond_broadcast(&lock->read_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}

// Operaciones de la lista enlazada
bool insert(int value, ListNode **head, RWLock *lock)
{
    rwlock_write_lock(lock);

    ListNode *curr = *head;
    ListNode *pred = nullptr;
    while (curr != nullptr && curr->data < value)
    {
        pred = curr;
        curr = curr->next;
    }

    if (curr == nullptr || curr->data > value)
    {
        ListNode *new_node = new ListNode{value, curr};
        if (pred == nullptr)
        {
            *head = new_node;
        }
        else
        {
            pred->next = new_node;
        }
        rwlock_write_unlock(lock);
        return true;
    }

    rwlock_write_unlock(lock);
    cout << value << " ya esta en la lista.\n";
    return false;
}

void print(ListNode *head, RWLock *lock)
{
    rwlock_read_lock(lock);

    cout << "Lista: ";
    ListNode *curr = head;
    while (curr != nullptr)
    {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << "\n";

    rwlock_read_unlock(lock);
}

bool search(int value, ListNode *head, RWLock *lock)
{
    rwlock_read_lock(lock);

    ListNode *curr = head;
    while (curr != nullptr && curr->data < value)
    {
        curr = curr->next;
    }

    if (curr != nullptr && curr->data == value)
    {
        rwlock_read_unlock(lock);
        cout << value << " esta en la lista.\n";
        return true;
    }

    rwlock_read_unlock(lock);
    cout << value << " no esta en la lista.\n";
    return false;
}

bool remove(int value, ListNode **head, RWLock *lock)
{
    rwlock_write_lock(lock);

    ListNode *curr = *head;
    ListNode *pred = nullptr;
    while (curr != nullptr && curr->data < value)
    {
        pred = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->data == value)
    {
        if (pred == nullptr)
        {
            *head = curr->next;
        }
        else
        {
            pred->next = curr->next;
        }
        delete curr;
        rwlock_write_unlock(lock);
        return true;
    }

    rwlock_write_unlock(lock);
    cout << value << " no esta en la lista.\n";
    return false;
}

// Programa principal
int main()
{
    ListNode *head = nullptr;
    RWLock lock;
    rwlock_init(&lock);

    int command;
    int value;

    while (true)
    {
        cout << "Comandos:\n1. Insertar\n2. Imprimir\n3. Buscar\n4. Eliminar\n5. Salir\n";
        cout << "Ingrese un comando: ";
        cin >> command;

        if (command == 5)
        {
            break;
        }

        switch (command)
        {
        case 1:
            cout << "Ingrese un valor para insertar: ";
            cin >> value;
            insert(value, &head, &lock);
            break;
        case 2:
            print(head, &lock);
            break;
        case 3:
            cout << "Ingrese un valor para buscar: ";
            cin >> value;
            search(value, head, &lock);
            break;
        case 4:
            cout << "Ingrese un valor para eliminar: ";
            cin >> value;
            remove(value, &head, &lock);
            break;
        default:
            cout << "Comando invalido.\n";
            break;
        }
    }

    // Liberar memoria
    while (head != nullptr)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}

// g++ -o ejercicio3 ejercicio3.cpp -pthread
