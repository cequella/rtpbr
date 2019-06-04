#pragma once

class IScene {
public:
	virtual ~IScene() {}

	virtual void onUpdate() = 0;
	virtual void onRender(double delay) = 0;
};