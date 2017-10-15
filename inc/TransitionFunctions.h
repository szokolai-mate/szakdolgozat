#pragma once

#include <TransitionFunction.h>

//! \~english Namespace containing TransitionFunction implementations.
//! \~hungarian TransitionFunction implementációkat tartalmazó névtér.
/*!
    \~english This namespace contains TransitionFunction implementations, grouped in sub-namespaces by the graph they describe.
    \~hungarian Ez a névtér TransitionFunction implementációkat tartalmaz, melyek a leírt gráf szerint csoportosítva vannak alnévterekbe.
*/
namespace Transition{
    //! \~english Simple linear transition.
    //! \~hungarian Egyszerű lineáris átmenet.
    /*!
        \~english The transition keeps a constant speed.
        \~hungarian Az átmenet tartja a sebességét.
    */
    template <typename T>
    class Linear : public TransitionFunction<T>{
    public:
        void init(const float & x1, const T & y1, const float & x2, const T & y2){
            this->x1 = x1;
            this->y1 = y1;
            this->m = (y2-y1)/(x2-x1);
        }

        inline T operator()(const float & x){
            return this->m * (x-this->x1) + this->y1;
        }
    };

    //! \~english Ease-in transitions.
    //! \~hungarian Elöl gyorsuló átmenetek.
    /*!
        \~english The transitions accelerate at the start.
        \~hungarian Az átmenet az elején gyorsul.
    */
    namespace EaseIn{
        //! \~english Quadratic ease-in transition.
        //! \~hungarian Négyzetes elöl gyorsuló átmenet.
        template <typename T>
        class Quadratic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1)/((x2-x1)*(x2-x1));
                this->x1=x1;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x1)) * (x-(this->x1)) + this->y1;
            }
        };

        //! \~english Cubic ease-in transition.
        //! \~hungarian Köbös elöl gyorsuló átmenet.
        template <typename T>
        class Cubic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1)/((x2-x1)*(x2-x1)*(x2-x1));
                this->x1=x1;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x1)) * (x-(this->x1))* (x-(this->x1)) + this->y1;
            }
        };

        //! \~english Quartic ease-in transition.
        //! \~hungarian Negyedik hatványos elöl gyorsuló átmenet.
        template <typename T>
        class Quartic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1)/((x2-x1)*(x2-x1)*(x2-x1)*(x2-x1));
                this->x1=x1;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x1)) * (x-(this->x1))* (x-(this->x1))* (x-(this->x1)) + this->y1;
            }
        };
        //! \~english Quintic ease-in transition.
        //! \~hungarian Ötödik hatványos elöl gyorsuló átmenet.
        template <typename T>
        class Quintic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1)/((x2-x1)*(x2-x1)*(x2-x1)*(x2-x1)*(x2-x1));
                this->x1=x1;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x1)) * (x-(this->x1))* (x-(this->x1))* (x-(this->x1))* (x-(this->x1)) + this->y1;
            }
        };
    };


    //! \~english Ease-out transitions.
    //! \~hungarian Hátul lassuló átmenetek.
    /*!
        \~english The transitions decelerate at the end.
        \~hungarian Az átmenet a végén lassul.
    */
    namespace EaseOut{
        //! \~english Quadratic ease-out transition.
        //! \~hungarian Négyzetes hátul lassuló átmenet.
        template <typename T>
        class Quadratic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y1-y2)/((x1-x2)*(x1-x2));
                this->x2=x2;
                this->y2=y2;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x2)) * (x-(this->x2)) + this->y2;
            }
        };
        
        //! \~english Cubic ease-out transition.
        //! \~hungarian Köbös hátul lassuló átmenet.
        template <typename T>
        class Cubic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y1-y2)/((x1-x2)*(x1-x2)*(x1-x2));
                this->x2=x2;
                this->y2=y2;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x2)) * (x-(this->x2))* (x-(this->x2)) + this->y2;
            }
        };

        //! \~english Quartic ease-out transition.
        //! \~hungarian Negyedik hatványos hátul lassuló átmenet.
        template <typename T>
        class Quartic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y1-y2)/((x1-x2)*(x1-x2)*(x1-x2)*(x1-x2));
                this->x2=x2;
                this->y2=y2;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x2)) * (x-(this->x2))* (x-(this->x2))* (x-(this->x2)) + this->y2;
            }
        };

        //! \~english Quintic ease-out transition.
        //! \~hungarian Ötödik hatványos hátul lassuló átmenet.
        template <typename T>
        class Quintic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y1-y2)/((x1-x2)*(x1-x2)*(x1-x2)*(x1-x2)*(x1-x2));
                this->x2=x2;
                this->y2=y2;
            }
            
            inline T operator()(const float & x){
                return (this->m) * (x-(this->x2)) * (x-(this->x2))* (x-(this->x2))* (x-(this->x2))* (x-(this->x2)) + this->y2;
            }
        };
    };

    //! \~english Ease-in-out transitions.
    //! \~hungarian Elöl gyorsuló, hátul lassuló átmenetek.
    /*!
        \~english The transitions accelerates at the start, reaches peak acceleration in the middle and decelerates at the end.
        \~hungarian Az átmenet az elején gyorsul, a közepén éri el a legnagyobb gyorsulást, majd a végén lassul.
    */
    namespace EaseInOut{
        //! \~english Quadratic ease-in-out transition.
        //! \~hungarian Négyzetes elöl gyorsuló, hátul lassuló átmenet.
        template <typename T>
        class Quadratic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1);
                this->x1=x1;
                this->x2=x2;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                float X = x-this->x1;
                return (this->m) * ((X*X)/((X*X)+((this->x2-this->x1)-X)*((this->x2-this->x1)-X))) + this->y1;
            }
        };
        
        //! \~english Cubic ease-in-out transition.
        //! \~hungarian Köbös elöl gyorsuló, hátul lassuló átmenet.
        template <typename T>
        class Cubic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1);
                this->x1=x1;
                this->x2=x2;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                float X = x-this->x1;
                return (this->m) * ((X*X*X)/((X*X*X)+((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X))) + this->y1;
            }
        };
        
        //! \~english Quartic ease-in-out transition.
        //! \~hungarian Negyedik hatványos elöl gyorsuló, hátul lassuló átmenet.
        template <typename T>
        class Quartic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1);
                this->x1=x1;
                this->x2=x2;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                float X = x-this->x1;
                return (this->m) * ((X*X*X*X)/((X*X*X*X)+((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X))) + this->y1;
            }
        };

        //! \~english Quintic ease-in-out transition.
        //! \~hungarian Ötödik hatványos elöl gyorsuló, hátul lassuló átmenet.
        template <typename T>
        class Quintic : public TransitionFunction<T>{
        public:
            void init(const float & x1, const T & y1, const float & x2, const T & y2){
                this->m = (y2-y1);
                this->x1=x1;
                this->x2=x2;
                this->y1=y1;
            }
            
            inline T operator()(const float & x){
                float X = x-this->x1;
                return (this->m) * ((X*X*X*X*X)/((X*X*X*X*X)+((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X)*((this->x2-this->x1)-X))) + this->y1;
            }
        };
    };
};