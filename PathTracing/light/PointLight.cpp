#include "PointLight.h"

Color3 PointLight::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	Color3 rgb;
	Material& intersectAttr = result.primitive->attr;
	Vec3&r = origin - result.point;
	double rr = Length(r);

	if(!scene->isInShadow(Ray(result.point,r),this))
	{
		Vec3 s = Normalize(r);
		
		if(intersectAttr.kd!=Color3::BLACK)
		{
			//calculate the diffuse color
			double mDots = Dot(s,result.normal);
			if(mDots>0.0) rgb+= mDots*intersectAttr.kd*intersectAttr.ka
				*intense/PI;
		}
		
		if(intersectAttr.ks!=Color3::BLACK)
		{
			//calculate the specular color
			Vec3 v = ray.direction.flip();
			Vec3 h = Normalize(s+v);
			double mDotH = Dot(h,result.normal);
			if(mDotH>0.0) rgb+= pow(mDotH,intersectAttr.shiness)*intersectAttr.ks
				*attr.emission*intense
				*(intersectAttr.shiness+1)/(2*PI);
		}
	}
	return rgb;
}