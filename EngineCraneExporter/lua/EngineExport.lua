-- Exporter code based off the ExportExample.lua file from 
-- https://github.com/EndfinityDev/csvExporter

SCRIPT_VERSION = 20240101

function DoExport(CarCalculator, CarFile)
	UAPI.Log("DoExport: ")

	local Data = CExporter.ExportCarData(CarCalculator)
	local Files = {}
	Files[string.format("%s - %s.car", CarCalculator.CarInfo.PlatformInfo.Name, CarCalculator.CarInfo.TrimInfo.Name)] = CarFile

	return Files, Data
end

if CExporter == nil then
	CExporter = {}
	CExporter.__index = CExporter
end


function CExporter.ExportCarData(CarCalculator)
	UAPI.Log("CalculateEngine: ")
	CarCalculator.EngineCalculator:CalculateEngine()

	local Data = {}
	for k,v in pairs(AddCurveData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddBasicData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddPartData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddTuningParameters(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddResultData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddEmissionsData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddQualityData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddTechPoolData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddFuelData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddFailureData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddNoiseData(CarCalculator)) do
		Data[k] = v
	end
	for k,v in pairs(AddProductionData(CarCalculator)) do
		Data[k] = v
	end

	return Data
end


function AddCurveData(CarCalculator)
    UAPI.Log("AddCurveData: ")
	local Data = {}
    local EngineResults = CarCalculator.CarInfo.TrimInfo.EngineInfo.ModelInfo.Results
    for idx, val in pairs(EngineResults.UE4Curves.RPM) do
	    local key_name = "Curve.RPM." .. idx
	    Data[key_name] = val
		local FrictionKey = "Curve.Friction." .. idx
		Data[FrictionKey] = CarCalculator.EngineCalculator:GetFrictionValues(val)
	end
	for idx, val in pairs(EngineResults.UE4Curves.Torque) do
	    local key_name = "Curve.Torque." .. idx
	    Data[key_name] = val
	end
	for idx, val in pairs(EngineResults.UE4Curves.Boost) do
	    local key_name = "Curve.Boost." .. idx
	    Data[key_name] = val
	end
	for idx, val in pairs(EngineResults.UE4Curves.TotalFriction) do
	    local key_name = "Curve.TotalFriction." .. idx
	    Data[key_name] = val
	end
	return Data
end

function AddBasicData(CarCalculator)
    UAPI.Log("AddBasicData: ")
	local tablePrefix = "Info."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo
	Data[tablePrefix .. "GameVersion"] = UAPI.GetGameVersion()
	Data[tablePrefix .. "ExporterScriptVersion"] = SCRIPT_VERSION
	Data[tablePrefix .. "EngineUID"] = EngineInfo.ModelInfo.UID
    Data[tablePrefix .. "FamilyYear"] = EngineInfo.PlatformInfo.Time.Date.Year
	Data[tablePrefix .. "FamilyName"] = EngineInfo.PlatformInfo.Name
	Data[tablePrefix .. "VariantYear"] = EngineInfo.ModelInfo.Time.Date.Year
	Data[tablePrefix .. "VariantName"] = EngineInfo.ModelInfo.Name
	return Data
end

function AddPartData(CarCalculator)
    UAPI.Log("AddPartData: ")
	local tablePrefix = "Parts."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo
    Data[tablePrefix .. "BlockType"] = EngineInfo.PlatformInfo.BlockType.Name
	Data[tablePrefix .. "Cylinders"] = EngineInfo.PlatformInfo.BlockConfig.Cylinders
	Data[tablePrefix .. "BlockMaterial"] = EngineInfo.PlatformInfo.BlockMaterial.Name
	Data[tablePrefix .. "HeadMaterial"] = EngineInfo.PlatformInfo.HeadMaterial.Name
	Data[tablePrefix .. "HeadType"] = EngineInfo.PlatformInfo.Head.Name
	Data[tablePrefix .. "IntakeValves"] = EngineInfo.PlatformInfo.Valves.IntakeValves
	Data[tablePrefix .. "ExhaustValves"] = EngineInfo.PlatformInfo.Valves.ExhaustValves
	Data[tablePrefix .. "BalanceShaft"] = EngineInfo.ModelInfo.BalanceShaft.Name
	Data[tablePrefix .. "BalancingMass"] = EngineInfo.ModelInfo.BalancingMassSetting
	Data[tablePrefix .. "Crankshaft"] = EngineInfo.ModelInfo.Crank.Name
	Data[tablePrefix .. "Conrods"] = EngineInfo.ModelInfo.Conrods.Name
	Data[tablePrefix .. "Pistons"] = EngineInfo.ModelInfo.Pistons.Name
    Data[tablePrefix .. "VVT"] = EngineInfo.ModelInfo.VVT.Name
	Data[tablePrefix .. "VVL"] = EngineInfo.ModelInfo.VVL.Name
	Data[tablePrefix .. "AspirationType"] = EngineInfo.ModelInfo.AspirationType.Name
	Data[tablePrefix .. "Aspiration"] = EngineInfo.ModelInfo.AspirationSetup.Name
	Data[tablePrefix .. "BoostControl"] = EngineInfo.ModelInfo.AspirationBoostControl.Name
    Data[tablePrefix .. "AspirationItem1"] = EngineInfo.ModelInfo.AspirationItemOption_1.Name
	Data[tablePrefix .. "AspirationSubItem1"] = EngineInfo.ModelInfo.AspirationItemSubOption_1.Name
	Data[tablePrefix .. "AspirationItem2"] = EngineInfo.ModelInfo.AspirationItemOption_2.Name
	Data[tablePrefix .. "AspirationSubItem2"] = EngineInfo.ModelInfo.AspirationItemSubOption_2.Name
	Data[tablePrefix .. "FuelSystemType"] = EngineInfo.ModelInfo.FuelSystemType.Name
	Data[tablePrefix .. "FuelSystem"] = EngineInfo.ModelInfo.FuelSystem.Name
	Data[tablePrefix .. "Throttles"] = EngineInfo.ModelInfo.IntakeManifold.Name
	Data[tablePrefix .. "Intake"] = EngineInfo.ModelInfo.Intake.Name
	Data[tablePrefix .. "Headers"] = EngineInfo.ModelInfo.Headers.Name
	Data[tablePrefix .. "Cat"] = EngineInfo.ModelInfo.Cat.Name
	Data[tablePrefix .. "Muffler1"] = EngineInfo.ModelInfo.Muffler1.Name
	Data[tablePrefix .. "Muffler2"] = EngineInfo.ModelInfo.Muffler2.Name
	return Data
end

function AddTuningParameters(CarCalculator)
    UAPI.Log("AddTuningParameters: ")
	local tablePrefix = "Tune."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo

    -- BottomEnd
    Data[tablePrefix .. "FamilyBore"] = EngineInfo.PlatformInfo.Bore
	Data[tablePrefix .. "FamilyStroke"] = EngineInfo.PlatformInfo.Stroke
	Data[tablePrefix .. "FamilyDisplacement"] = EngineInfo.PlatformInfo.Capacity
	Data[tablePrefix .. "Bore"] = EngineInfo.ModelInfo.Bore
	Data[tablePrefix .. "Stroke"] = EngineInfo.ModelInfo.Stroke
	Data[tablePrefix .. "Displacement"] = EngineInfo.ModelInfo.Capacity
	
	-- TopEnd
	Data[tablePrefix .. "Compression"] = EngineInfo.ModelInfo.Compression
	Data[tablePrefix .. "CamProfile"] = EngineInfo.ModelInfo.CamProfileSetting
	Data[tablePrefix .. "ValveSpringStiffness"] = EngineInfo.ModelInfo.SpringStiffnessSetting
	Data[tablePrefix .. "VVLCamProfile"] = EngineInfo.ModelInfo.VVLCamProfileSetting or 0
	Data[tablePrefix .. "VVLRPM"] = EngineInfo.ModelInfo.VVLRPM or 0
	
	-- Aspiration
	Data[tablePrefix .. "IntercoolerSize"] = EngineInfo.ModelInfo.IntercoolerSetting
	Data[tablePrefix .. "ChargerSize1"] = EngineInfo.ModelInfo.ChargerSize_1
	Data[tablePrefix .. "ChargerSize2"] = EngineInfo.ModelInfo.ChargerSize_2
	Data[tablePrefix .. "TurbineSize1"] = EngineInfo.ModelInfo.TurbineSize_1
	Data[tablePrefix .. "TurbineSize2"] = EngineInfo.ModelInfo.TurbineSize_2
	Data[tablePrefix .. "ChargerTune1"] = EngineInfo.ModelInfo.ChargerTune_1
	Data[tablePrefix .. "ChargerTune2"] = EngineInfo.ModelInfo.ChargerTune_2
	Data[tablePrefix .. "ChargerMaxBoost1"] = EngineInfo.ModelInfo.ChargerMaxBoost_1
	Data[tablePrefix .. "ChargerMaxBoost2"] = EngineInfo.ModelInfo.ChargerMaxBoost_2
	
	-- FuelSystem
	Data[tablePrefix .. "CarbSize"] = EngineInfo.ModelInfo.CarbSize or 0
	Data[tablePrefix .. "IntakeSize"] = EngineInfo.ModelInfo.IntakeSize or 0
	Data[tablePrefix .. "FuelMap"] = EngineInfo.ModelInfo.FuelMap or 0
	Data[tablePrefix .. "RPMLimit"] = EngineInfo.ModelInfo.RPMLimit
	
	-- Exhaust
	Data[tablePrefix .. "HeaderSize"] = EngineInfo.ModelInfo.HeaderSize
	Data[tablePrefix .. "FamilyExhaustCountBore"] = EngineInfo.ModelInfo.ExhaustCount.Name
	Data[tablePrefix .. "BypassValves"] = EngineInfo.ModelInfo.ExhaustBypassValves.Name
	Data[tablePrefix .. "ExhaustDiameter"] = EngineInfo.ModelInfo.ExhaustDiameter
	return Data
end

function AddResultData(CarCalculator)
    UAPI.Log("AddResultData: ")
	local tablePrefix = "Results."
	local Data = {}
    local EngineResults = CarCalculator.CarInfo.TrimInfo.EngineInfo.ModelInfo.Results
    Data[tablePrefix .. "PeakPower"] = EngineResults.PeakPower
	Data[tablePrefix .. "PeakPowerRPM"] = EngineResults.PeakPowerRPM
	Data[tablePrefix .. "PeakTorque"] = EngineResults.PeakTorque
	Data[tablePrefix .. "PeakTorqueRPM"] = EngineResults.PeakTorqueRPM
	Data[tablePrefix .. "MaxRPM"] = EngineResults.MaxRPM
	Data[tablePrefix .. "IdleRPM"] = EngineResults.Idle
	Data[tablePrefix .. "PeakBoost"] = EngineResults.PeakBoost
	Data[tablePrefix .. "PeakBoostRPM"] = EngineResults.PeakBoostRPM
	Data[tablePrefix .. "Weight"] = EngineResults.Weight
	Data[tablePrefix .. "FlywheelWeight"] = EngineResults.FlyWheelWeight
	Data[tablePrefix .. "RequiredCooling"] = EngineResults.CoolingRequired
	Data[tablePrefix .. "PerformanceIndex"] = EngineResults.PerformanceIndex
	Data[tablePrefix .. "Responsiveness"] = EngineResults.Responsiveness
	Data[tablePrefix .. "Smoothness"] = EngineResults.Smoothness
	Data[tablePrefix .. "MTTF"] = EngineResults.MTTF
	Data[tablePrefix .. "EconEff"] = EngineResults.EconEff
	Data[tablePrefix .. "RON"] = EngineResults.RON
	return Data
end

function AddEmissionsData(CarCalculator)
    UAPI.Log("AddEmissionsData: ")
	local tablePrefix = "Emissions."
	local Data = {}
    Data[tablePrefix .. "WES"] = CarCalculator.CarInfo.TrimInfo.Results.PassedEmissionsName
	local wltp3 = CarCalculator.CarInfo.TrimInfo.Results.EmissionsWLTP_Class3
	Data[tablePrefix .. "CO"] = wltp3.CO
	Data[tablePrefix .. "COPerKm"] = wltp3.COPerKm
	Data[tablePrefix .. "NOx"] = wltp3.NOx
	Data[tablePrefix .. "NOxPerKm"] = wltp3.NOxPerKm
	Data[tablePrefix .. "HC"] = wltp3.HC
	Data[tablePrefix .. "HCPerKm"] = wltp3.HCPerKm
	return Data
end

function AddQualityData(CarCalculator)
    UAPI.Log("AddQualityData: ")
	local tablePrefix = "Quality."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo
	Data[tablePrefix .. "Family"] = EngineInfo.PlatformInfo.QualitySettings.Family
	Data[tablePrefix .. "TopEnd"] = EngineInfo.ModelInfo.QualitySettings.TopEnd
	Data[tablePrefix .. "BottomEnd"] = EngineInfo.ModelInfo.QualitySettings.BottomEnd
	Data[tablePrefix .. "Aspiration"] = EngineInfo.ModelInfo.QualitySettings.Aspiration
	Data[tablePrefix .. "FuelSystem"] = EngineInfo.ModelInfo.QualitySettings.FuelSystem
	Data[tablePrefix .. "Exhaust"] = EngineInfo.ModelInfo.QualitySettings.Exhaust
	return Data
end

function AddTechPoolData(CarCalculator)
    UAPI.Log("AddTechPoolData: ")
	local tablePrefix = "TechPool."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo
	Data[tablePrefix .. "Family"] = EngineInfo.PlatformInfo.TechPool.Family
	--Data.FamilyTopEndTechpool = EngineInfo.PlatformInfo.TechPool.TopEnd
	Data[tablePrefix .. "TopEnd"] = EngineInfo.ModelInfo.TechPool.TopEnd
	Data[tablePrefix .. "VFamily"] = EngineInfo.ModelInfo.TechPool.Family
	Data[tablePrefix .. "BottomEnd"] = EngineInfo.ModelInfo.TechPool.BottomEnd
	Data[tablePrefix .. "Aspiration"] = EngineInfo.ModelInfo.TechPool.Aspiration
	Data[tablePrefix .. "FuelSystem"] = EngineInfo.ModelInfo.TechPool.FuelSystem
	Data[tablePrefix .. "Exhaust"] = EngineInfo.ModelInfo.TechPool.Exhaust
	return Data
end

function AddFuelData(CarCalculator)
    UAPI.Log("AddFuelData: ")
	local tablePrefix = "Fuel."
	local Data = {}
    local EngineInfo = CarCalculator.CarInfo.TrimInfo.EngineInfo
    Data[tablePrefix .. "Type"] = EngineInfo.ModelInfo.Fuel.Type
	Data[tablePrefix .. "Leaded"] = EngineInfo.ModelInfo.Fuel.FuelLeaded == true and "TRUE" or "FALSE"
	Data[tablePrefix .. "Octane"] = EngineInfo.ModelInfo.Fuel.ListedOctane
	Data[tablePrefix .. "OctaneOffset"] = EngineInfo.ModelInfo.Fuel.TuneOctaneOffset
	return Data
end

function AddFailureData(CarCalculator)
    UAPI.Log("AddFailureData: ")
	local tablePrefix = "Fail."
	local Data = {}
    local EngineResults = CarCalculator.CarInfo.TrimInfo.EngineInfo.ModelInfo.Results
    Data[tablePrefix .. "CrankMaxRPM"] = EngineResults.PartsFailureInfo.Crank.RPM
	Data[tablePrefix .. "CrankMaxTorque"] = EngineResults.PartsFailureInfo.Crank.Torque
	Data[tablePrefix .. "ConrodMaxRPM"] = EngineResults.PartsFailureInfo.Conrods.RPM
	Data[tablePrefix .. "ConrodMaxTorque"] = EngineResults.PartsFailureInfo.Conrods.Torque
	Data[tablePrefix .. "PistonMaxRPM"] = EngineResults.PartsFailureInfo.Pistons.RPM
	Data[tablePrefix .. "PistonMaxTorque"] = EngineResults.PartsFailureInfo.Pistons.Torque
	--Data[tablePrefix .. "TurboMaxRPM"] = EngineResults.PartsFailureInfo.Turbo.RPM
	return Data
end

function AddNoiseData(CarCalculator)
    UAPI.Log("AddNoiseData: ")
	local tablePrefix = "Noise."
	local Data = {}
    local EngineResults = CarCalculator.CarInfo.TrimInfo.EngineInfo.ModelInfo.Results
    Data[tablePrefix .. "EngineNoise"] = EngineResults.Noise
	Data[tablePrefix .. "IntakeNoise"] = EngineResults.IntakeNoise
	Data[tablePrefix .. "ExhaustNoise"] = EngineResults.ExhaustNoise
	return Data
end

function AddProductionData(CarCalculator)
    UAPI.Log("AddProductionData: ")
	local tablePrefix = "Production."
	local Data = {}
    local EngineResults = CarCalculator.CarInfo.TrimInfo.EngineInfo.ModelInfo.Results
    Data[tablePrefix .. "EngineeringTime"] = EngineResults.EngineeringTime
	Data[tablePrefix .. "PersonHours"] = EngineResults.ManHours
	
	Data[tablePrefix .. "EngineeringCost"] = EngineResults.EngineeringCost
	Data[tablePrefix .. "ToolingCosts"] = EngineResults.ToolingCosts
	Data[tablePrefix .. "MaterialCost"] = EngineResults.MaterialCost
	Data[tablePrefix .. "TotalCost"] = EngineResults.TotalCost
	Data[tablePrefix .. "ServiceCost"] = EngineResults.ServiceCost

    --Cost calculations
	local function CalculateCost(EngineResults, employeeCount, employeeWage, automationCoef, shiftCount)
	    local engineeringTime = EngineResults.EngineeringTime
		local productionUnits = EngineResults.ManHours
		local engineeringCost = EngineResults.EngineeringCost
		local materialCost = EngineResults.MaterialCost
		local toolingCosts = EngineResults.ToolingCosts

		local employeeCostsPerShift = employeeCount * employeeWage * 8
		local factoryProductionUnits = productionUnits / (employeeCount * automationCoef)

		local carsMadePerShift = 8 / factoryProductionUnits
		local carsMadePerDay = carsMadePerShift * shiftCount
		local employeeCostsPerDay = employeeCostsPerShift * shiftCount

		local carsMadePerMonth = carsMadePerDay * 30
		local employeeCostsPerMonth = employeeCostsPerDay * 30
		local employeeCostsPerCar = employeeCostsPerShift / carsMadePerShift

		local monthlyEngineeringCosts = engineeringCost / 60
		local engineeringCostsPerCar = monthlyEngineeringCosts / carsMadePerMonth

		local toolingCosts = toolingCosts * (shiftCount / 2)

		local totalCostPerEngine = materialCost + engineeringCostsPerCar + employeeCostsPerCar + toolingCosts

		return totalCostPerEngine
	end

	Factory = {SHED = 10, TINY = 50, SMALL = 100, MEDIUM = 150, LARGE = 200, HUGE = 500}
	Labor = {SWEATSHOP = 5, CHEAP = 10, AVERAGE = 20, EXPENSIVE = 30}
	Automation = {NONE = 1, LOW = 1.2, MEDIUM = 1.5, HIGH = 2}

	Data[tablePrefix .. "CostPreset0"] = CalculateCost(EngineResults, Factory.LARGE, Labor.CHEAP, Automation.MEDIUM, 2)
	Data[tablePrefix .. "CostPreset1"] = CalculateCost(EngineResults, Factory.HUGE, Labor.CHEAP, Automation.MEDIUM, 2)
	Data[tablePrefix .. "CostPreset2"] = CalculateCost(EngineResults, Factory.MEDIUM, Labor.AVERAGE, Automation.HIGH, 2)
	Data[tablePrefix .. "CostPreset3"] = CalculateCost(EngineResults, Factory.SMALL, Labor.AVERAGE, Automation.HIGH, 3)
	Data[tablePrefix .. "CostPreset4"] = CalculateCost(EngineResults, Factory.TINY, Labor.AVERAGE, Automation.MEDIUM, 2)
	Data[tablePrefix .. "CostPreset5"] = CalculateCost(EngineResults, Factory.TINY, Labor.CHEAP, Automation.MEDIUM, 3)
	Data[tablePrefix .. "CostPreset6"] = CalculateCost(EngineResults, Factory.SHED, Labor.CHEAP, Automation.NONE, 2)
	Data[tablePrefix .. "CostPreset7"] = CalculateCost(EngineResults, Factory.SHED, Labor.AVERAGE, Automation.NONE, 2)
	Data[tablePrefix .. "CostPreset8"] = CalculateCost(EngineResults, Factory.SHED, Labor.EXPENSIVE, Automation.NONE, 1)
	Data[tablePrefix .. "CostPreset9"] = CalculateCost(EngineResults, Factory.LARGE, Labor.SWEATSHOP, Automation.LOW, 3)
	Data[tablePrefix .. "CostPreset10"] = CalculateCost(EngineResults, Factory.HUGE, Labor.SWEATSHOP, Automation.LOW, 3)
	Data[tablePrefix .. "CostPreset11"] = CalculateCost(EngineResults, Factory.HUGE, Labor.AVERAGE, Automation.MEDIUM, 2)
	Data[tablePrefix .. "CostPreset12"] = CalculateCost(EngineResults, Factory.TINY, Labor.EXPENSIVE, Automation.HIGH, 2)
	Data[tablePrefix .. "CostPreset13"] = CalculateCost(EngineResults, Factory.SHED, Labor.EXPENSIVE, Automation.HIGH, 2)

	return Data
end
