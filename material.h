#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"




class material  {
    public:
        material(const double& kd, const double& ks, const double& ka,
                const color& od, const color& os, const double& kgls) : 
                kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls) {refl = 0; ri=0;}
        material(const double& kd, const double& ks, const double& ka,
            const color& od, const color& os, const double& kgls, const double& refl) : 
            kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls), refl(refl) {ri=0;}
        material(const double& kd, const double& ks, const double& ka,
            const color& od, const color& os, const double& kgls, const double& refl, const double& ri) : 
            kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls), refl(refl), ri(ri) {}

    virtual double getKD() const {return kd;}
    virtual double getKA() const {return ka;}
    virtual double getKS() const {return ks;}
    virtual color getOD() const {return od;}
    virtual color getOS() const {return os;}
    virtual double getKgls() const {return kgls;}
    virtual double getRefl() const {return refl;}
    virtual double getRI() const {return ri;}

    private:
        double kd;
        double ks;
        double ka;
        color od; //diffuse color
        color os; //specular color
        double kgls; //gloss coefficient
        double refl;
        double ri;
};

#endif