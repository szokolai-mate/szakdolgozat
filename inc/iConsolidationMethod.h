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
        \~english \param initial the initializing vector
                        \~hungarian \param initial a kezdeti érték
        \~english \return whether initialization was successful
                \~hungarian \return az inicializáció sikeres volt-e
    */
        virtual bool init(const std::vector<T> & initial) = 0;
    /*!
        \~english Add a vector to the consolidation.
        \~hungarian Vektor hozzáadása a konszolidációhoz.
       \~english  \param vector the vector to add
                        \~hungarian  \param vector a hozzáadandó vektor
        \~english \return whether adding the vector was successful
                \~hungarian \return sikeres volt-e a hozzáadás
    */
        virtual bool add(const std::vector<T> & vector) = 0;
    /*!
        \~english Get the result of the consolidation. Only usable once.
        \~hungarian Az eredmény lekérése. Egyszer használható.

        \~english \return the result of the consolidation
                \~hungarian \return a konszolidáció eredménye
    */
        virtual std::vector<T> get() = 0;
        virtual ~iConsolidationMethod(){};
    };
};