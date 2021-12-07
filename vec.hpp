#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <iostream>

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
typedef triple<int> color;
//Décrit un point (x,y,z) dans l'espace
typedef triple<double> point;
//Décrit un vecteur (x,y,z) dans l'espace
typedef triple<double> vec;

//Surcharge flux de données en écriture pour afficher les triplets
template<class charT, class charTraits, typename T>
std::basic_ostream<charT>& operator <<(std::basic_ostream<charT, charTraits>& theStream, const triple<T>& aTriple)
{
    theStream << "(" << aTriple.getX() << "," << aTriple.getY() << "," << aTriple.getZ() << ")" << std::endl; 
    return theStream;
}


//Création d'une classe rayon, ayant un point d'origine et un vecteur direction
class ray{
    private:
        point origin;
        vec direction;
        //Nombre max de reflexion d'un rayon
        int max_reflection;
    public:
        ray(){}
        ray(point anOrigin, vec aDirection, int aMax_reflection) : origin(anOrigin), direction(aDirection), max_reflection(aMax_reflection){}
        ray(const ray &aVector) : origin(aVector.origin), direction(aVector.direction), max_reflection(aVector.max_reflection){}
        ~ray(){}
        //Méthodes de lecture
        point getOrigin(){
            return origin;
        }
        vec getDirection(){
            return direction;
        }
        int getMax_reflection(){
            return max_reflection;
        }
        //Méthode de déplacement le long du rayon
        point move(double step){
            return origin + direction*step;
        }
};



#endif