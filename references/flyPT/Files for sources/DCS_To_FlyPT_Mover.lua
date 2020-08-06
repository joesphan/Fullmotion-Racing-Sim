--[[

DCS LUA SCRIPT FOR FLYPT MOVER 2.9.8
====================================
Version from 18 04 2020
This file should be placed in the folder "C:\Users\YOUR USERNAME\Saved Games\DCS\Scripts\Hooks".
If the "Hooks" folder doen's exist, create it.

]]--

local FlyPT_Mover_Callbacks = {}

function FlyPT_Mover_Callbacks.onSimulationStart()

	log.write('FlyPT_Mover', log.INFO, "Starting data export")
	package.path = package.path..";.\\LuaSocket\\?.lua"
	package.cpath = package.cpath..";.\\LuaSocket\\?.dll"
    socket = require("socket")
    DCSClient = socket.udp()
    DCSClient:settimeout(0)
	DCSIP = "127.0.0.1"
	DCSPort = 4124 -- If changed, the source should match this value
    DCSClient:setpeername(DCSIP, DCSPort)
		
end

function FlyPT_Mover_Callbacks.onSimulationFrame()

	local acceleration = Export.LoGetAccelerationUnits()  
	local speed = Export.LoGetVectorVelocity()
    local pitch, roll, yaw = Export.LoGetADIPitchBankYaw()
    local rotationSpeed = Export.LoGetAngularVelocity()
	local altitude = Export.LoGetAltitudeAboveGroundLevel()
	local o = Export.LoGetSelfData()
	--[[
	LatLongAlt.Lat -- Latitude in degress
	LatLongAlt.Long -- Longitude in degress
	LatLongAlt.Alt -- Altitude in meters MSL
	Heading -- Heading in radians
	Pitch -- Pitch in radians
	Bank -- Bank in radians
	]]--
	
	local mechInfo=Export.LoGetMechInfo()
	--[[
	gear          = {status,value,main = {left = {rod},right = {rod},nose =  {rod}}}
	flaps		  = {status,value}  
	speedbrakes   = {status,value}
	refuelingboom = {status,value}
	airintake     = {status,value}
	noseflap      = {status,value}
	parachute     = {status,value}
	wheelbrakes   = {status,value}
	hook          = {status,value}
	wing          = {status,value}
	canopy        = {status,value}
	controlsurfaces = {elevator = {left,right},eleron = {left,right},rudder = {left,right}}
	]]--
	
	local alarm = Export.LoGetMCPState()
	--[[
		returned table keys for LoGetMCPState():
		"LeftEngineFailure"
		"RightEngineFailure"
		"HydraulicsFailure"
		"ACSFailure"
		"AutopilotFailure"
		"AutopilotOn"
		"MasterWarning"
		"LeftTailPlaneFailure"
		"RightTailPlaneFailure"
		"LeftAileronFailure"
		"RightAileronFailure"
		"CanopyOpen"
		"CannonFailure"
		"StallSignalization"
		"LeftMainPumpFailure"
		"RightMainPumpFailure"
		"LeftWingPumpFailure"
		"RightWingPumpFailure"
		"RadarFailure"
		"EOSFailure"
		"MLWSFailure"
		"RWSFailure"
		"ECMFailure"
		"GearFailure"
		"MFDFailure"
		"HUDFailure"
		"HelmetFailure"
		"FuelTankDamage"
	]]--
	local stall = 0
	for k,v in pairs(alarm) do
		if k == "StallSignalization" then 
			if v == true then
				stall = 1
			end
		end
	end

	-- local engine = LoGetEngineInfo()
	--[[
	RPM = {left, right},(%)
	Temperature = { left, right}, (Celcium degrees)
	HydraulicPressure = {left ,right},kg per square centimeter
	FuelConsumption   = {left ,right},kg per sec
	fuel_internal      -- fuel quantity internal tanks	kg
	fuel_external      -- fuel quantity external tanks	kg	
	]]--

	-- The FlyPT Mover uses Z for vertical and Y to the front
	-- That's the opposite in DCS
	-- Values sent in one string, separated by spaces	
	socket.try(DCSClient:send(
	--               00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15   16   17   18   19   20   21   22   23
	string.format("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f", 
	acceleration.x, 							-- 00 = Lateral acceleration (G)
	acceleration.z, 							-- 01 = Lateral acceleration (G)
	acceleration.y, 							-- 02 = Vertical acceleration (G)
	speed.x, 									-- 03 = Lateral speed (m/s)
	speed.z, 									-- 04 = Longitudinal speed (m/s)
	speed.y, 									-- 05 = Vertical speed (m/s)
	rotationSpeed.y, 							-- 06 = Rotation speed around y (yaw in rad/s)
	rotationSpeed.x, 							-- 07 = Rotation speed around x (roll in rad/s)
	rotationSpeed.z, 							-- 08 = Rotation speed around z (pitch in rad/s)
	o.Heading, 									-- 09 = Yaw position (rad)
	o.Bank, 									-- 10 = Roll position (rad)
	o.Pitch, 									-- 11 = Pitch position (rad)
	Export.LoGetTrueAirSpeed(), 				-- 12 = Air speed (m/s)
	Export.LoGetAircraftDrawArgumentValue(1), 	-- 13 = Front/Rear landing gear (0 to 1)?
	Export.LoGetAircraftDrawArgumentValue(2), 	-- 14 = Turning landing gear (0 to 1)?
	Export.LoGetAircraftDrawArgumentValue(4),	-- 15 = Left landing gear (0 to 1)?
	Export.LoGetAircraftDrawArgumentValue(6), 	-- 16 = Right landing gear (0 to 1)?
	Export.LoGetAltitudeAboveGroundLevel(), 	-- 17 = Vertical position relative to ground (m)
	mechInfo.flaps.value, 						-- 18 = Flaps amount (%)
	mechInfo.gear.value, 						-- 19 = Delployed landing gear (%)
	mechInfo.speedbrakes.value, 				-- 20 = Speed brakes (%)
	mechInfo.canopy.value, 						-- 21 = Canopy open (%)
	stall, 										-- 22 = Stall alarm (0 or 1)
	Export.LoGetModelTime() 					-- 23 = Time in seconds
	)))
end

function FlyPT_Mover_Callbacks.onSimulationStop()
	log.write('FlyPT_Mover', log.INFO, "Data export stopped")
	if DCSClient then
        DCSClient:close()
    end
end

DCS.setUserCallbacks(FlyPT_Mover_Callbacks)