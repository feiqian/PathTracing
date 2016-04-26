#include "ILight.h"

class AmbientLight : public ILight
{
public:
	AmbientLight(Color3 color);
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);
private:
	Color3 color;
};