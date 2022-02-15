#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <iostream>
#include <ostream>
#include <cmath>

//Classe générale pour un triplet
template <typename T>
class triple{
    friend class ray;
    private:
        T x;
        T y;
        T z;
    public:
        triple(): x(0), y(0), z(0){}
        triple(T xi, T yi,T zi): x(xi), y(yi), z(zi){}
        triple<T>(const triple<T> &aTriple): x(aTriple.x), y(aTriple.y), z(aTriple.z){}
        ~triple(){}
        //Méthodes de lecture et d'écritures sur les attributs
        T getX() const{
            return x;
        }
        T getY() const{
            return y;
        }
        T getZ() const{
            return z;
        }
        void setX(T aX){
            x = aX;
        }
        void setY(T aY){
            y = aY;
        }
        void setZ(T aZ){
            z = aZ;
        }
        //Méthodes de calcul vectoriel: addition, soustraction, multiplication, division, produit scalaire, produit vectoriel et norme
        triple operator +(const triple<T>& aTriple){
            triple result = *this;
            result.x += aTriple.x;
            result.y += aTriple.y;
            result.z += aTriple.z;
            return result;
        }
        triple operator -(const triple<T>& aTriple){
            triple result = *this;
            result.x -= aTriple.x;
            result.y -= aTriple.y;
            result.z -= aTriple.z;
            return result;
        }
        triple operator *(const double a){
            triple result = *this;
            result.x *= a;
            result.y *= a;
            result.z *= a;
            return result;
        }
        triple operator /(const double a){
            triple result = *this;
            if(a!=0){
                result.x *= 1/a;
                result.y *= 1/a;
                result.z *= 1/a;
                return result;
            }else{
                std::cout << "Error. Division by zero" << std::endl;
                return result;
            }
        }
        double operator *(const triple<T>& aTriple){
            return this->x*aTriple.x + this->y*aTriple.y + this->z*aTriple.z;
        }
        triple prod_vect(const triple<T>& aTriple){
            triple result;
            result.x = y*aTriple.z - z*aTriple.y;
            result.y = z*aTriple.x - x*aTriple.z;
            result.z = x*aTriple.y - y*aTriple.x;
            return result; 
        }
        double norm(){
            return sqrt(x*x + y*y + z*z);
        }
};

//Création d'alias sur les triplets pour bien définir les objets utilisés dans le programme
//Décrit les couleurs en RGB
typedef triple<double> color;
//Décrit un point (x,y,z) dans l'espace
typedef triple<double> point;
//Décrit un vecteur (x,y,z) dans l'espace
typedef triple<double> vec;


//Surcharge flux de données en écriture pour afficher les triplets
template<typename T>
std::ostream& operator <<(std::ostream& theStream, const triple<T>& aTriple)
{
    theStream << "(" << aTriple.getX() << "," << aTriple.getY() << "," << aTriple.getZ() << ")"; 
    return theStream;
}


#endif