ctrluser@devDesk:~$ export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python
ctrluser@devDesk:~$ python
Python 3.9.2 (default, Feb 28 2021, 17:03:44)
[GCC 10.2.1 20210110] on linux
Type "help", "copyright", "credits" or "license" for more information.

$>sudo pip install aapy

>>> from aa.utils import utc_datetime
>>> from aa.js import JsonFetcher
>>>
>>> from datetime import datetime
>>> event = js.get_event_at('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', datetime.now())
>>> js = JsonFetcher('192.168.75.70', 17668)
>>> event = js.get_event_at('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', datetime.now())
WARNING:root:Assuming timezone for 2023-04-18 23:21:57.601018 is Asia/Seoul
>>> event.value
array([14.28513432])
>>> event.utc_datetime
datetime.datetime(2023, 4, 18, 14, 21, 51, 780416, tzinfo=<UTC>)
>>> data = js.get_values('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', utc_datetime(2023,4,18,11,20,00), utc_datetime(2023,4,18,11,25,00))
