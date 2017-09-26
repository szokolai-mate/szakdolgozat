#pragma once

#include <iConsolidationMethod.h>

//! \~english Classes implementing consolidation methods, used by consolidators.
//! \~hungarian Konszolidációs módszereket megvalósító osztályok, melyeket majd a konszolidátorok használnak.
/*!
    \~english Consolidation Methods should implement the iConsolidationMethod interface.
    \~hungarian Az osztályok az iConsolidationMethod interfészből származtathatóak.
*/
namespace Consolidation{
    //! \~english An element in the resulting vector will be the sum of all the elements in the same positions.
    //! \~hungarian Az eredményben minden elem az elemmel azonos pozición lévő elemek összege.
    /*!
        \~english For example: [1,2][1,2][2,2] -> [4,6]
        \~hungarian Például: [1,2][1,2][2,2] -> [4,6]
    */
    template <typename T>
    class Accumulation : public DataFlow::iConsolidationMethod<T>{
    private:
        std::vector<T> workspace;
    public:
        bool init(const std::vector<T> & initial);
        bool add(const std::vector<T> & vector);
        std::vector<T> get();
    };
};