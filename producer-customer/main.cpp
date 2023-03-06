#include <mutex>
#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;

static const int bufferSize = 10;
static const int itemTotal = 20;

struct Buffer
{
    int buffer[bufferSize];
    size_t readPosition, writePosition;
    mutex mtx;
    condition_variable notFull, notEmpty;
} buffer;

typedef struct Buffer Buffer;

void ProduceItem(Buffer *buf, int item)
{
    unique_lock<mutex> lock(buf->mtx);
    while ((buf->writePosition + 1) % bufferSize == buf->readPosition)
    {
        cout << "buffer is full now,producer is waiting ......" << endl;
        buf->notFull.wait(lock);
    }
    buf->buffer[buf->writePosition] = item;
    buf->writePosition++;
    if (buf->writePosition >= bufferSize)
    {
        buf->writePosition = 0;
    }
    buf->notEmpty.notify_all();
    lock.unlock();
}

void ConsumeItem(Buffer *buf, int &data)
{
    unique_lock<mutex> lock(buf->mtx);
    while (buf->writePosition == buf->readPosition)
    {
        cout << "buffer is empty, consumer is waiting ......" << endl;
        buf->notEmpty.wait(lock);
    }
    data = buf->buffer[buf->readPosition];
    buf->readPosition++;
    if (buf->readPosition >= bufferSize)
    {
        buf->readPosition = 0;
    }
    buf->notFull.notify_all();
    lock.unlock();
}

// 生产者任务
void producer()
{
    for (int i = 1; i <= itemTotal; i++)
    {
        cout << "prodece the " << i << "^th item ..." << endl;
        ProduceItem(&buffer, i);
    }
}

// 消费者任务
void consumer()
{
    static int cnt = 0;
    int item;
    while (1)
    {
        ConsumeItem(&buffer, item);
        cout << "consume the " << item << "^th item" << endl;
        if (++cnt == itemTotal)
            break;
    }
}

// 初始化 buffer
void init_buffer(Buffer *b)
{
    b->writePosition = 0;
    b->readPosition = 0;
}

int main()
{
    init_buffer(&buffer);
    thread prodece(producer);
    thread consume(consumer);
    prodece.join();
    consume.join();
}