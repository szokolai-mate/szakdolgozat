#pragma once

#include <vector>

/*! \~english Contains classes pertaining to the control of flow and application of transformers of data.
            Made to be as generic as possible.
    \~hungarian Az adat folyásával és transzformálásával kapcsolatos osztályokat tartalmaz.
            A lehető leggernerikusabban megvalósítva.
*/
namespace DataFlow
{
//! \~english Interface for a source of data of type T in the data flow.
//! \~hungarian Interface egy T típusú adatforráshoz az adatfolyamban.
template <typename T>
class iSource
{
  public:
    /*! \~english Get data from this source.
            \~hungarian Adatot kér ettől a forrástól.

            \~english \param amount the amount of type T to get
            \~hungarian \param amount a kért adat mennyisége
            \~english \return a vector of type **T** and of size **amount** containing the data
            \~hungarian \return egy **T** típusú, **amount** méretű vektor, feltöltve az adattal.
        */
    virtual std::vector<T> get(const unsigned int &amount) = 0;

    virtual ~iSource(){};
};
};