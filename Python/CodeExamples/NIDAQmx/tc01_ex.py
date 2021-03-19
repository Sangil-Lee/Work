import nidaqmx

task = nidaqmx.Task()

task.ai_channels.add_ai_thrmcpl_chan("TC01/ai0")

task.start()

value = task.read()
print(round(value,1))

task.stop()
task.close()