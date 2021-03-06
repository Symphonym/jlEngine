/*

	The MIT License (MIT)

	Copyright (c) 2014 by Jakob Larsson

	Permission is hereby granted, free of charge, to any person obtaining 
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software is 
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */


#ifndef SAUROBYTE_AUDIO_DEVICE_HPP
#define SAUROBYTE_AUDIO_DEVICE_HPP

#include <Saurobyte/AudioChunk.hpp>
#include <Saurobyte/AudioStream.hpp>
#include <Saurobyte/Priority.hpp>
#include <string>
#include <memory>
#include <vector>

namespace Saurobyte
{
	// Handles to audio data
	typedef std::shared_ptr<AudioSource> AudioHandle;

	namespace internal
	{
		class OpenALImpl;
	};

	class AudioDevice
	{

	public:

		~AudioDevice();

		// Add channel support
		// store audio etc values by mapped channel names
		// set these values to audios put in that channel TODO

		/**
		 * Register an audio file to a unique string identifier
		 * @param fileName Audio file path
		 * @param name     Audio file identifier
		 */
		static void registerAudio(const std::string &fileName, const std::string &name);

		static AudioHandle createStream(const std::string &name, PriorityType priority = Priority::Medium);
		static AudioHandle playStream(const std::string &name, PriorityType priority = Priority::Medium);

		static AudioHandle createSound(const std::string &name, PriorityType priority = Priority::Medium);
		static AudioHandle playSound(const std::string &name, PriorityType priority = Priority::Medium);

		/**
		 * Set the playback device to be used for audio output
		 * @param playbackDevice Name of the playback device
		 */
		void setPlaybackDevice(const std::string &playbackDevice);
		/**
		 * Set the capture device used for audio input
		 * @param captureDevice Name of the capture device
		 */
		void setCaptureDevice(const std::string &captureDevice);

		/**
		 * Stops all audio playback
		 */
		static void stopAllAudio();

		/**
		 * Gets the total amount of sounds/streams stored in memory 
		 * @return The total amount of sounds
		 */
		static std::size_t getTotalSoundCount();

		static std::vector<std::string> getPlaybackDevices();
		static std::vector<std::string> getCaptureDevices();

	private:

		// Only the Engine can create an AudioDevice
		friend class Engine;

		std::unique_ptr<internal::OpenALImpl> m_openAL;

		AudioDevice();

		// Deleletes unused audio memory, TODO
		static void bufferCleanup();

		// Grabs an OpenAL source handle to use
		static std::uint32_t grabAudioSource();

		// Finds the least important source that is currently active
		static std::uint32_t leastImportantSource();

		// Wipe the specified source's data to default
		static void wipeSource(std::uint32_t source);
	};
};

#endif