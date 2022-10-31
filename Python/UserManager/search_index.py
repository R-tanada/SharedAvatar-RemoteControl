import pyaudio

iaudio = pyaudio.PyAudio()
for i in range(0, iaudio.get_device_count()):
    print(iaudio.get_device_info_by_index(i))