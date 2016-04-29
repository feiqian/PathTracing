#include "ILight.h"

class AmbientLight : public ILight
{
public:
	AmbientLight(Color3 emission);
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);
private:
	Color3 emission;
};