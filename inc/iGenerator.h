#pragma once

template <typename T>
class iGenerator {
    virtual T next() = 0;
}