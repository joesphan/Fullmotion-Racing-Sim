# BeamNG.drive data decoding

Motion simulators are a relatively niche community, and it seems as though if there is no standard protocol for communication between games and motion sims. The proprietary protocols used by games are also massively undocumented. 

After getting help from the BeamNG forums, I discovered that the entirety of the MotionSim sending module is a Lua script located in (BeamInstallDIrectory)\lua\vehicle\motionSim.lua

Here is a code snippet that is relevant, I have deleted code unnecessary to understand the encoding, my comments are prefaced with a -- JL --:
```lua

-- This Source Code Form is subject to the terms of the bCDDL, v. 1.1.
-- If a copy of the bCDDL was not distributed with this
-- file, You can obtain one at http://beamng.com/bCDDL-1.1.txt

local ffi = require("ffi")

local function sendDataPaketV1(dt)
  local o = ffi.new("motionSim_t")
-- JL -- the packet is built using ffi.
    --Byte 0
  o.magic = "BNG1"
    --Byte 3
    --Byte 4
  o.posX, o.posY, o.posZ = obj:getPositionXYZ()
    --Byte 15
    --Byte 16
  o.velX = velocity.x
  o.velY = velocity.y
  o.velZ = velocity.z
    --Byte 27
    --Byte 28
  o.accX = accX
  o.accY = accY
  o.accZ = accZ
    --Byte 39
    --Byte 40
  o.upVecX = upVector.x
  o.upVecY = upVector.y
  o.upVecZ = upVector.z
    --Byte 51
    --Byte 52
  o.rollPos = -euler.z --negated angle here, seems like that is the "standard" for motion sims here
  o.pitchPos = -euler.y --negated angle here, seems like that is the "standard" for motion sims here
  o.yawPos = euler.x
    --Byte 63
    --Byte 64
  o.rollRate = rollRate
  o.pitchRate = pitchRate
  o.yawRate = yawRate
    --Byte 75
    --Byte 76
  o.rollAcc = (rollRate - lastFrameData.rollRate) / dt
  o.pitchAcc = (pitchRate - lastFrameData.pitchRate) / dt
  o.yawAcc = (yawRate - lastFrameData.yawRate) / dt
    --Byte 88
  local packet = ffi.string(o, ffi.sizeof(o))
-- JL -- repack the packet by exporting the ffi to string
   
  udpSocket:sendto(packet, ip, port)
-- JL -- send the packet via UDP

end
```



Here I have the decoded data. the data that concerns me is underlined and verified. Special thanks to [FlyPT on BeamNG forums for pre-decoding lots of it](https://www.beamng.com/threads/motionsim-enabled.66137/)

I had to verify Bytes 28 to 39, and 40 to 51


Byte 0 to 3 - "BNG1" 					//most likely a signifier for what version of BeamNG motionsim encoding

Byte 4 to 7 - Float - X position in world coordinates - m

Byte 8 to 11 - Float - Y position in world coordinates - m

Byte 12 to 15 - Float - Z position in world coordinates - m

Byte 16 to 19 - Float - Lateral speed in local coordinates - m/s

Byte 20 to 23- Float - Longitudinal speed in local coordinates - m/s

Byte 24 to 27- Float - Vertical speed in local coordinates - m/s



<u>Byte 28 to 31- Float - Side to side acceleration - m/s/s</u>

<u>Byte 32 to 35 - Float - Forward acceleration -m/s/s</u>

Byte 36 to 39 - Float - Vertical acceleration in local coordinates -m/s/s



Byte 40 to 51 - ? - Can it be accelerations with gravity

Byte 40 to 51 - ? - Can it be accelerations with gravity

Byte 40 to 51 - ? - Can it be accelerations with gravity ?



<u>Byte 52 to 55 - Float - Roll - rad</u>

<u>Byte 56 to 59 - Float - Pitch - rad</u>



Byte 60 to 63 - Float - Yaw - rad







Byte 64 to 67 - Float - Roll speed - rad/s

Byte 68 to 71- Float - Pitch speed - rad/s

Byte 72 to 75- Float - Yaw speed - rad/s

Byte 76 to 79 - Float - Roll acceleration - rad/ss

Byte 80 to 83 - Float - Pitch acceleration - rad/ss

Byte 84 to 88 - Float - Yaw acceleration - rad/ss



# Verifying FlyPT's work

There is too much data to verify, so I will only verify the ones that matter

I used WireShark to scan the data coming in from the local UDP port.

**pitch:**

```
0000   02 00 00 00 45 00 00 74 1f 04 00 00 80 11 00 00
0010   7f 00 00 01 7f 00 00 01 ce bc 11 5c 00 60 07 06


0020   42 4e 47 31 fb dc a9 c1 0c 9c 5e c3 6e 45 21 41
0030   61 20 87 38 c3 f8 26 3a 78 02 ef b9 26 d2 86 ba
0040   b0 d5 69 ba 83 c6 f0 ba 26 5a d4 b9 69 05 4a 3f
0050   a8 3c 1d 3f e0 81 87 3b **0d c6 68 3f** 68 c2 48 c0
0060   ab 4d 88 b8 4c b1 6d b9 ce b9 f0 b8 f1 5c 0d bd
0070   a9 6f 19 3d e4 9f 21 be
```

0d c6 68 3f = 0.909272 little endian, in radians (float), which is 52.1 degrees.

Byte 56 to 59 - Float - Pitch - rad



**roll:**

```0000   02 00 00 00 45 00 00 74 7f 6e 00 00 80 11 00 00
0000   02 00 00 00 45 00 00 74 2c 51 00 00 80 11 00 00
0010   7f 00 00 01 7f 00 00 01 f2 7f 11 5c 00 60 39 e0


0020   42 4e 47 31 52 6f 18 43 64 80 26 c2 fb 8b 41 40
0030   98 9f 1f 39 94 a5 30 39 99 6b 57 38 f0 25 f6 3a
0040   4c 59 cb 39 58 47 54 3a b9 67 ef be 31 bd 6f bc
0050   60 42 62 3f **2e 12 f8 be** 3c be 4a bd dd 59 ff bd
0060   fc bd 25 37 72 a3 fb 37 03 82 06 39 2a 00 c3 3d
0070   e4 e6 24 be 5a 2a 6c 3e
```

2e 12 f8 be = -0.484514 little endian, in radians (float), which is -27.7 degrees.

Byte 52 to 55 - Float - Roll - rad



 **Horizontal G's**

```
0000   02 00 00 00 45 00 00 74 11 7c 00 00 80 11 00 00
0010   7f 00 00 01 7f 00 00 01 f6 af 11 5c 00 60 b2 38


0020   42 4e 47 31 7b a0 12 44 60 80 23 c3 e7 32 5e 3e
0030   5b b7 bf c0 60 53 3c c1 58 dd 97 bb **51 1e ed 40**
0040   40 27 fe bd a5 77 8c be 3c 63 0d 3d 0a e1 b7 bc
0050   6c c8 7f 3f 9a 5e 27 bd 4a f1 9f bb 57 33 2c c0
0060   6b 89 12 bd 92 00 b6 bd 98 32 19 3f 9d e8 11 c2
0070   08 8b e6 c2 48 7b 84 41
```

`51 1e ed 40` = 7.40995   = -10.9051 m/s/s Float, little endian - side to side acceleration

**Forward G's**

```
0000   02 00 00 00 45 00 00 74 47 ae 00 00 80 11 00 00
0010   7f 00 00 01 7f 00 00 01 db 60 11 5c 00 60 8c 2f


0020   42 4e 47 31 68 c2 a9 c4 fe 6f bc 43 c0 21 da 3d
0030   7e 03 2e c2 d1 12 a2 3f 48 2c 95 bb 57 27 99 be
0040   **51 7b 2e c1** e9 8f f3 be f0 0a ff bc 7d b1 aa b9
0050   3b e0 7f 3f c0 3d 92 ba 37 73 fb bc 00 d0 c5 bf
0060   96 a2 a1 3b 3d d6 48 bd d3 5c 08 bc 95 a4 0a 42
0070   45 34 b1 c2 8a 7f 6e c1
```

`51 7b 2e c1 `  = -10.9051 m/s/s Float, little endian - acceleration/braking

Byte 32 to 35

the eSBR pulls roughly 1 G's acceleration, so this is accurate

**Vertical G's**

 ```
0000   02 00 00 00 45 00 00 74 e6 6c 00 00 80 11 00 00
0010   7f 00 00 01 7f 00 00 01 cf f5 11 5c 00 60 03 13


0020   42 4e 47 31 97 4f 5b 43 d5 0c b1 c3 f8 5a 28 44
0030   48 ef a2 bc 4c 7c 93 42 0b 06 2f c2 4e 56 b8 3e
0040   f1 18 a2 40 **f3 ee 01 c1** 3f 0c 57 3d 81 8e 25 3f
0050   58 cc 42 3f 87 ac 08 3d 27 95 34 bf b3 33 f2 3c
0060   59 9c 7e 3c 24 47 84 be 73 31 4d 3c 56 26 17 be
0070   87 75 c0 bf d3 fe 2e 3d
 ```

`f3 ee 01 c1` = -8.12084 m/s/s m/s/s Float, little endian - roughly gravity

Byte 36 to 39

