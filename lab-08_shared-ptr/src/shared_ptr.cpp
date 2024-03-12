#include "matrix.hpp"
#include "shared_ptr.hpp"
#include <stdexcept>
#include <algorithm>

shared_ptr::Storage::Storage(Matrix *mtx) : data_(mtx), ref_count_(1)
{}

shared_ptr::Storage::~Storage()
{
    delete data_;
}

void shared_ptr::Storage::incr()
{
    ++ref_count_;
}

void shared_ptr::Storage::decr()
{
    --ref_count_;
}

int shared_ptr::Storage::getCounter() const
{
    return ref_count_;
}

Matrix *shared_ptr::Storage::getObject() const
{
    return data_;
}

shared_ptr::shared_ptr(Matrix *obj)
{
    storage_ = new Storage(obj);
}

shared_ptr::shared_ptr(const shared_ptr &other)
{
    storage_ = other.storage_;
    storage_->incr();
}

shared_ptr &shared_ptr::operator=(const shared_ptr &other)
{
    if (this != &other)
    {
        processTheDeletion();
        storage_ = other.storage_;
        storage_->incr();
    }
    return *this;

}

shared_ptr::~shared_ptr()
{
    processTheDeletion();
}

Matrix *shared_ptr::ptr() const
{
    return storage_->getObject();
}

bool shared_ptr::isNull() const
{
    return storage_->getObject() == nullptr;
}

void shared_ptr::reset(Matrix *obj)
{
    processTheDeletion();
    storage_ = new Storage(obj);
}


Matrix *shared_ptr::operator->() const
{
    return ptr();
}

Matrix &shared_ptr::operator*() const
{
    if (isNull())
    {
        throw std::runtime_error("Attempted to dereference a nullptr\n");
    }
    return *ptr();
}

void shared_ptr::processTheDeletion()
{
    storage_->decr();
    if (storage_->getCounter() <= 0)
    {
        delete storage_;
    }
}
