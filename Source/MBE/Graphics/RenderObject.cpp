#include <MBE/Graphics/RenderObject.h>

using namespace mbe;

RenderObject::RenderObject(RenderLayer renderLayer, float zOrder) :
	renderLayer(renderLayer),
	zOrder(zOrder)
{
}

RenderObject::RenderLayer mbe::operator++(RenderObject::RenderLayer & renderLayer)
{
	return renderLayer = static_cast<RenderObject::RenderLayer>(static_cast<std::underlying_type<RenderObject::RenderLayer>::type>(renderLayer) + 1);
}
