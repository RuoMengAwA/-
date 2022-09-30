import configparser, time, hmac, hashlib, logging, os, sys, random
from linkkit import linkkit

lk = linkkit.LinkKit(
    host_name="cn-shanghai",
    product_key="gzxr4QABkPX",
    device_name="Jetson_NX",
    device_secret="aec6d241e333dc9d1f1cca8bfca26810")


def on_device_dynamic_register(rc, value, userdata):
    if rc == 0:
        print("dynamic register device success, rc:%d, value:%s" % (rc, value))
    else:
        print("dynamic register device fail,rc:%d, value:%s" % (rc, value))


def on_connect(session_flag, rc, userdata):
    print("on_connect:%d,rc:%d,userdata:" % (session_flag, rc))
    pass

def on_disconnect(rc, userdata):
    print("on_disconnect:rc:%d,userdata:" % rc)


def on_publish_topic(mid, userdata):
    print("on_publish_topic mid:%d" % mid)


def on_thing_enable(self, userdata):
    print("on_thing_enable")


def on_thing_disable(self, userdata):
    print("on_thing_disable")


def on_thing_prop_post(self, request_id, code, data, message, userdata):
    print("on_thing_prop_post request id:%s, code:%d, data:%s message:%s" %
          (request_id, code, str(data), message))


if __name__ == '__main__':
    print("hello")

    lk.on_device_dynamic_register = on_device_dynamic_register
    lk.on_connect = on_connect
    lk.on_disconnect = on_disconnect
    lk.on_publish_topic = on_publish_topic
    lk.on_thing_enable = on_thing_enable
    lk.on_thing_disable = on_thing_disable
    lk.on_thing_prop_post = on_thing_prop_post
    lk.thing_setup("222.json")
    lk.connect_async()
    lk.start_worker_loop()
    time.sleep(1)  # 延时

    prop_data = {
        "VehInsideTemp": 22,
        "Humidity": 20,
        "blo_ac": 0.14,
        "HeartRate": 80,
        "blo": 97,
        "blp": 112.23,
        "blp_l": 80.23,
        "bls": 91,
        "step": 2320,
        "cal": 2220,
        "air_ac":0.000146,
        "dan_bev": 0,
        "SDNN": 113,
        "pilao":0
    }
    rc, request_id = lk.thing_post_property(prop_data)


    for i in range(0,100):
        i= i-1
        print(prop_data)

