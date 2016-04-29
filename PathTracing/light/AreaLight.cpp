#include "AreaLight.h"

AreaLight::AreaLight(Point3 origin,Vec3 dx,Vec3 dy,Color3 emission):origin(origin),dx(dx),dy(dy),emission(emission)
{
	lightSamples = 3;
	normal = Cross(dx,dy);
	area =  Length(normal)*0.5;
	normal = Normalize(normal);
}

Color3 AreaLight::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	if(emission==Color3::BLACK ) return Color3::BLACK;

	Color3 rgb;
	Material& intersectAttr = result.primitive->attr;
	Reflectance& ref = result.primitive->getReflectance(result.point);
	double rate = 1.0 / lightSamples;

	for (int i = 0; i < lightSamples; i++) {
		double sx = (double)rand() / RAND_MAX;
		double sy = (double)rand() / RAND_MAX * (1.0-sx);

		Vec3 lightOrigin = origin + dx * sx + dy * sy;
		Vec3&r = lightOrigin - result.point;
		double rr = Length(r);
		Ray& shadowRay = Ray(result.point,r);
		shadowRay.tMax = rr;

		if(!scene->isInShadow(shadowRay))
		{
			Vec3 s = Normalize(r);

			double cosThetaIn = max(Dot(result.normal,s), 0.0);
			double cosThetaOut = max(Dot(-s,normal), 0.0);
			double geoFactor = cosThetaIn*cosThetaOut / (rr*rr);
			// Probability: 1/area.
			Vec3 intensity = geoFactor * area * emission * rate;

			if(ref.kd!=Color3::BLACK)
			{
				//calculate the diffuse color
				double mDots = Dot(s,result.normal);
				if(mDots>0.0) rgb+= mDots*ref.kd
					*intensity/PI;
			}

			if(ref.ks!=Color3::BLACK)
			{
				//calculate the specular color
				Vec3 v = ray.direction.flip();
				Vec3 h = Normalize(s+v);
				double mDotH = Dot(h,result.normal);
				if(mDotH>0.0) rgb+= pow(mDotH,intersectAttr.shiness)*ref.ks
					*intensity
					*(intersectAttr.shiness+1)/(2*PI);
			}
		}
	}

	return rgb;
}
