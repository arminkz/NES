#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;

protected:
	Renderer();

public:

	virtual ~Renderer();

	void initialize();

	void render();

	void sizeChanged(const uint16_t width, const uint16_t height);

};