#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"




class material  {
    public:
        material(const double& kd, const double& ks, const double& ka,
                const color& od, const color& os, const double& kgls) : 
                kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls) {}

    virtual double getKD() const {return kd;}
    virtual double getKA() const {return ka;}
    virtual double getKS() const {return ks;}
    virtual color getOD() const {return od;}
    virtual color getOS() const {return os;}
    virtual double getKgls() const {return kgls;}

    private:
        double kd;
        double ks;
        double ka;
        color od; //diffuse color
        color os; //specular color
        double kgls; //gloss coefficient
};

#endif