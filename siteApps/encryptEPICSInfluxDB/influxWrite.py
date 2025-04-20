import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

#token = os.environ.get("l_HnOIjEGflnDEJDRMjWV_cmMCtm2Yrgf9oh1osPoE04egQorYqlvzCTCSnyqXfzMEjadPaClVcU2MjlIV-tVA==")
#token = "COWyuJkBwwRcfv_GarnCr0KRnuBz4IF3bzAOv7T59VhRF_yPPUQ7jkTfH0NrP1rT7eG__b5bvMH6PYpBekwVYw=="
#token = "l_HnOIjEGflnDEJDRMjWV_cmMCtm2Yrgf9oh1osPoE04egQorYqlvzCTCSnyqXfzMEjadPaClVcU2MjlIV-tVA=="
token = os.environ.get("INFLUXDB_TOKEN")
print(token)
#token = "COWyuJkBwwRcfv_GarnCr0KRnuBz4IF3bzAOv7T59VhRF_yPPUQ7jkTfH0NrP1rT7eG__b5bvMH6PYpBekwVYw=="
org = "cotlab"
url = "http://localhost:8086"

client = InfluxDBClient(url=url, token=token, org=org)

bucket="ctrluser"

write_api = client.write_api(write_options=SYNCHRONOUS)
   
for value in range(100):
  point = (
    Point("pv1")
    .tag("tagname1", "tagvalue1")
    .field("field1", value)
  )
  write_api.write(bucket=bucket, org="cotlab", record=point)
  time.sleep(0.01) # separate points by 1 second


query_api = client.query_api()

query = """from(bucket: "ctrluser")
 |> range(start: -10m)
 |> filter(fn: (r) => r._measurement == "pv1")"""
tables = query_api.query(query, org="cotlab")

for table in tables:
  for record in table.records:
    print(record)

