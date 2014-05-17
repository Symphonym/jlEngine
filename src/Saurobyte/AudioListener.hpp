#ifndef JL_AUDIOLISTENER_HPP
#define JL_AUDIOLISTENER_HPP
#include <Saurobyte/Math/Vector3.hpp>

namespace Saurobyte
{
	namespace AudioListener
	{
		void setVolume(float volume);
		void setPosition(const Vector3f &position);

		void setDirection(const Vector3f &direction);
		void setUpVector(const Vector3f &up);


		float getVolume();
		const Vector3f& getPosition();
	};
};

#endif