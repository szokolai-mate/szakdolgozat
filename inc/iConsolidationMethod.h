#pragma once

#include <vector>

namespace DataFlow{
    template <typename T>
    //! \~english Interface required for consolidation methods used by Consolidator.
    //! \~hungarian A konszolidációs módszerek interfésze.
    /*!
        \~english For implemented consolidation methods see the Consolidation namespace.
        \~hungarian Konszolidációs módszerek implementálásához lásd a Consolidation névteret.
    */
    class iConsolidationMethod {
    public:
    /*!
        \~english Initialize the consolidation with the first vector.
        \~hungarian A konszolidáció kezdeti értékének megadása.
        \param initial \~english the initializing vector
                        \~hungarian a kezdeti érték
        \return \~english whether initialization was successful
                \~hungarian az inicializáció sikeres volt-e
    */
        virtual bool init(const std::vector<T> & initial) = 0;
    /*!
        \~english Add a vector to the consolidation.
        \~hungarian Vektor hozzáadása a konszolidációhoz.
        \param vector \~english the vector to add
                        \~hungarian a hozzáadandó vektor
        \return \~english whether adding the vector was successful
                \~hungarian sikeres volt-e a hozzáadás
    */
        virtual bool add(const std::vector<T> & vector) = 0;
    /*!
        \~english Get the result of the consolidation. Only usable once.
        \~hungarian Az eredmény lekérése. Egyszer használható.

        \return \~english the result of the consolidation
                \~hungarian a konszolidáció eredménye
    */
        virtual std::vector<T> get() = 0;
        virtual ~iConsolidationMethod(){};
    };
};