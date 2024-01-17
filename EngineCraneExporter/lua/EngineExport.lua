-- Exporter code based off the ExportExample.lua file from 
-- https://github.com/EndfinityDev/csvExporter
---------------------------------------------------
-- Copyright (c) 2021 -- Camshaft Software PTY LTD
---------------------------------------------------

function DoExport(CarCalculator, CarFile)
	UAPI.Log("DoExport: ")

	local value = {}

	local CarData = CExporter.ExportCarData(CarCalculator)
	local CarFiles = CExporter.ExportCarFiles(CarCalculator)
	CarFiles[string.format("%s - %s.car", CarCalculator.CarInfo.PlatformInfo.Name, CarCalculator.CarInfo.TrimInfo.Name)] = CarFile

	local Data = {}

	for k,v in pairs(CarData) do
		Data[k] = v
	end

	local Files = {}

	for k,v in pairs(CarFiles) do
		Files[k] = v
	end

	return Files, Data
end

if CExporter == nil then
	CExporter = {}
	CExporter.__index = CExporter
end

function CExporter.ExportCarData(CarCalculator)
	local CarInfo = CarCalculator.CarInfo
	local EngineInfo = CarInfo.TrimInfo.EngineInfo
	local Results = CarInfo.TrimInfo.Results

	local carParameters = CarCalculator:GetCarParameters()

	local Data = {}
	Data.TrimYear = CarInfo.TrimInfo.Time.Date.Year
	Data.ModelYear = CarInfo.PlatformInfo.Time.Date.Year
	Data.ModelName = CarInfo.PlatformInfo.Name
	Data.TrimName = CarInfo.TrimInfo.Name

	Data.FamilyYear = EngineInfo.PlatformInfo.Time.Date.Year
	Data.FamilyName = EngineInfo.PlatformInfo.Name
	Data.FamilyBore = EngineInfo.PlatformInfo.Bore
	Data.FamilyStroke = EngineInfo.PlatformInfo.Stroke
	Data.FamilyDisplacement = EngineInfo.PlatformInfo.Capacity

	Data.BlockType = EngineInfo.PlatformInfo.BlockType.Name
	Data.Cylinders = EngineInfo.PlatformInfo.BlockConfig.Cylinders
	Data.BlockMaterial = EngineInfo.PlatformInfo.BlockMaterial.Name
	
	Data.HeadMaterial = EngineInfo.PlatformInfo.HeadMaterial.Name
	Data.HeadType = EngineInfo.PlatformInfo.Head.Name
	Data.IntakeValves = EngineInfo.PlatformInfo.Valves.IntakeValves
	Data.ExhaustValves = EngineInfo.PlatformInfo.Valves.ExhaustValves
	
	Data.FamilyQuality = EngineInfo.PlatformInfo.QualitySettings.Family
	Data.FamilyTechpool = EngineInfo.PlatformInfo.TechPool.Family
	--Data.FamilyTopEndTechpool = EngineInfo.PlatformInfo.TechPool.TopEnd
	
	Data.VariantYear = EngineInfo.ModelInfo.Time.Date.Year
	Data.VariantName = EngineInfo.ModelInfo.Name
	Data.VariantBore = EngineInfo.ModelInfo.Bore
	Data.VariantStroke = EngineInfo.ModelInfo.Stroke
	Data.VariantDisplacement = EngineInfo.ModelInfo.Capacity
	
	Data.BalanceShaft = EngineInfo.ModelInfo.BalanceShaft.Name
	Data.BalancingMass = EngineInfo.ModelInfo.BalancingMassSetting
	Data.Crankshaft = EngineInfo.ModelInfo.Crank.Name
	Data.Conrods = EngineInfo.ModelInfo.Conrods.Name
	Data.Pistons = EngineInfo.ModelInfo.Pistons.Name
	
	Data.Compression = EngineInfo.ModelInfo.Compression
	Data.CamProfile = EngineInfo.ModelInfo.CamProfileSetting
	Data.ValveSpringStiffness = EngineInfo.ModelInfo.SpringStiffnessSetting
	
	Data.VVT = EngineInfo.ModelInfo.VVT.Name
	Data.VVL = EngineInfo.ModelInfo.VVL.Name
	Data.VVLCamProfile = EngineInfo.ModelInfo.VVLCamProfileSetting or 0
	Data.VVLRPM = EngineInfo.ModelInfo.VVLRPM or 0
	
	Data.Aspiration = EngineInfo.ModelInfo.AspirationSetup.Name
	Data.IntercoolerSize = EngineInfo.ModelInfo.IntercoolerSetting
	
	Data.AspirationItem1 = EngineInfo.ModelInfo.AspirationItemOption_1.Name
	Data.AspirationSubItem1 = EngineInfo.ModelInfo.AspirationItemSubOption_1.Name
	Data.AspirationItem2 = EngineInfo.ModelInfo.AspirationItemOption_2.Name
	Data.AspirationSubItem2 = EngineInfo.ModelInfo.AspirationItemSubOption_2.Name
	
	Data.BoostControl = EngineInfo.ModelInfo.AspirationBoostControl.Name
	Data.ChargerSize1 = EngineInfo.ModelInfo.ChargerSize_1
	Data.ChargerSize2 = EngineInfo.ModelInfo.ChargerSize_2
	Data.TurbineSize1 = EngineInfo.ModelInfo.TurbineSize_1
	Data.TurbineSize2 = EngineInfo.ModelInfo.TurbineSize_2
	Data.ChargerTune1 = EngineInfo.ModelInfo.ChargerTune_1
	Data.ChargerTune2 = EngineInfo.ModelInfo.ChargerTune_2
	Data.ChargerMaxBoost1 = EngineInfo.ModelInfo.ChargerMaxBoost_1
	Data.ChargerMaxBoost2 = EngineInfo.ModelInfo.ChargerMaxBoost_2
	
	Data.FuelSystemType = EngineInfo.ModelInfo.FuelSystemType.Name
	Data.FuelSystem = EngineInfo.ModelInfo.FuelSystem.Name
	Data.Throttles = EngineInfo.ModelInfo.IntakeManifold.Name
	Data.CarbSize = EngineInfo.ModelInfo.CarbSize or 0
	Data.Intake = EngineInfo.ModelInfo.Intake.Name
	Data.IntakeSize = EngineInfo.ModelInfo.IntakeSize or 0
	
	Data.FuelLeaded = EngineInfo.ModelInfo.Fuel.FuelLeaded == true and "TRUE" or "FALSE"
	Data.FuelOctane = EngineInfo.ModelInfo.Fuel.ListedOctane
	Data.OctaneOffset = EngineInfo.ModelInfo.Fuel.TuneOctaneOffset
	Data.FuelType = EngineInfo.ModelInfo.Fuel.Type
	
	--Data.AFR = EngineInfo.ModelInfo.AFR
	--Data.IgnitionTiming = EngineInfo.ModelInfo.IgnitionTimingSetting
	Data.FuelMap = EngineInfo.ModelInfo.FuelMap or 0
	
	Data.RPMLimit = EngineInfo.ModelInfo.RPMLimit
	
	Data.Headers = EngineInfo.ModelInfo.Headers.Name
	Data.HeaderSize = EngineInfo.ModelInfo.HeaderSize
	Data.ExhaustCount = EngineInfo.ModelInfo.ExhaustCount.Name
	Data.BypassValves = EngineInfo.ModelInfo.ExhaustBypassValves.Name
	Data.ExhaustDiameter = EngineInfo.ModelInfo.ExhaustDiameter
	Data.Cat = EngineInfo.ModelInfo.Cat.Name
	Data.Muffler1 = EngineInfo.ModelInfo.Muffler1.Name
	Data.Muffler2 = EngineInfo.ModelInfo.Muffler2.Name
	
	Data.VariantTopEndQuality = EngineInfo.ModelInfo.QualitySettings.TopEnd
	Data.VariantBottomEndQuality = EngineInfo.ModelInfo.QualitySettings.BottomEnd
	Data.VariantAspirationQuality = EngineInfo.ModelInfo.QualitySettings.Aspiration
	Data.VariantFuelSystemQuality = EngineInfo.ModelInfo.QualitySettings.FuelSystem
	Data.VariantExhaustQuality = EngineInfo.ModelInfo.QualitySettings.Exhaust
	
	Data.VariantTopEndTechpool = EngineInfo.ModelInfo.TechPool.TopEnd
	Data.VariantFamilyTechpool = EngineInfo.ModelInfo.TechPool.Family
	Data.VariantBottomEndTechpool = EngineInfo.ModelInfo.TechPool.BottomEnd
	Data.VariantAspirationTechpool = EngineInfo.ModelInfo.TechPool.Aspiration
	Data.VariantFuelSystemTechpool = EngineInfo.ModelInfo.TechPool.FuelSystem
	Data.VariantExhaustTechpool = EngineInfo.ModelInfo.TechPool.Exhaust
	
	Data.EnginePower = EngineInfo.ModelInfo.Results.PeakPower
	Data.EnginePowerRPM = EngineInfo.ModelInfo.Results.PeakPowerRPM
	Data.EngineTorque = EngineInfo.ModelInfo.Results.PeakTorque
	Data.EngineTorqueRPM = EngineInfo.ModelInfo.Results.PeakTorqueRPM
	Data.EngineMaxRPM = EngineInfo.ModelInfo.Results.MaxRPM
	Data.EngineIdleRPM = EngineInfo.ModelInfo.Results.Idle
	Data.EnginePeakBoost = EngineInfo.ModelInfo.Results.PeakBoost
	Data.EnginePeakBoostRPM = EngineInfo.ModelInfo.Results.PeakBoostRPM
	
	--Data.MinEconomyRPM = EngineInfo.ModelInfo.Results.MinEconRPM
	
	Data.EngineWeight = EngineInfo.ModelInfo.Results.Weight
	Data.EngineFlywheelWeight = EngineInfo.ModelInfo.Results.FlyWheelWeight
	Data.EngineRequiredCooling = EngineInfo.ModelInfo.Results.CoolingRequired
	Data.EnginePerformanceIndex = EngineInfo.ModelInfo.Results.PerformanceIndex
	Data.EngineResponsiveness = EngineInfo.ModelInfo.Results.Responsiveness
	Data.EngineSmoothness = EngineInfo.ModelInfo.Results.Smoothness

	Data.EngineReliability = EngineInfo.ModelInfo.Results.MTTF

	--Data.EngineEmissions = EngineInfo.ModelInfo.Results.Emissions

	--local allEmissionCycles = CarInfo.TrimInfo.Results.AllEmissionsCycles
	--local wesLevel = 0

	--for i, v in ipairs(allEmissionCycles) do
	--	local tp = type(v)
	--	local vd = dump(v)
	--	local br = tp + vd
	--	if v.Passrate >= 100 then
	--		wesLevel = math.max(wesLevel, i)
	--	end
	--end

	Data.WES = CarInfo.TrimInfo.Results.PassedEmissionsName
	
	local wltp3 = CarInfo.TrimInfo.Results.EmissionsWLTP_Class3
	Data.CO = wltp3.CO
	Data.COPerKm = wltp3.COPerKm
	Data.NOx = wltp3.NOx
	Data.NOxPerKm = wltp3.NOxPerKm
	Data.HC = wltp3.HC
	Data.HCPerKm = wltp3.HCPerKm

	Data.EngineEfficiency = EngineInfo.ModelInfo.Results.EconEff
	Data.EngineNoise = EngineInfo.ModelInfo.Results.Noise
	Data.IntakeNoise = EngineInfo.ModelInfo.Results.IntakeNoise
	Data.ExhaustNoise = EngineInfo.ModelInfo.Results.ExhaustNoise
	
	Data.EngineRONResult = EngineInfo.ModelInfo.Results.RON
	
	Data.EngineEngineeringTime = EngineInfo.ModelInfo.Results.EngineeringTime
	Data.EngineProductionUnits = EngineInfo.ModelInfo.Results.ManHours
	
	Data.EngineEngineeringCost = EngineInfo.ModelInfo.Results.EngineeringCost
	Data.EngineToolingCosts = EngineInfo.ModelInfo.Results.ToolingCosts
	Data.EngineMaterialCost = EngineInfo.ModelInfo.Results.MaterialCost
	Data.EngineTotalCost = EngineInfo.ModelInfo.Results.TotalCost
	
	Data.EngineServiceCost = EngineInfo.ModelInfo.Results.ServiceCost
	
	Data.CrankMaxRPM = EngineInfo.ModelInfo.Results.PartsFailureInfo.Crank.RPM
	Data.CrankMaxTorque = EngineInfo.ModelInfo.Results.PartsFailureInfo.Crank.Torque
	Data.ConrodMaxRPM = EngineInfo.ModelInfo.Results.PartsFailureInfo.Conrods.RPM
	Data.ConrodMaxTorque = EngineInfo.ModelInfo.Results.PartsFailureInfo.Conrods.Torque
	Data.PistonMaxRPM = EngineInfo.ModelInfo.Results.PartsFailureInfo.Pistons.RPM
	Data.PistonMaxTorque = EngineInfo.ModelInfo.Results.PartsFailureInfo.Pistons.Torque
	--Data.TurboMaxRPM = EngineInfo.ModelInfo.Results.PartsFailureInfo.Turbo.RPM	

	Data.GameVersion = UAPI.GetGameVersion()

	Data.EngineUID = EngineInfo.ModelInfo.UID

	--Cost calculations
	local function CalculateCost(engineeringTime, productionUnits, engineeringCost, materialCost, toolingCosts, 
								 employeeCount, employeeWage, automationCoef, shiftCount)
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

		local totalCostPerCar = materialCost + engineeringCostsPerCar + employeeCostsPerCar + toolingCosts

		return totalCostPerCar
	end --CalculateCost

		--Data.EngineEngineeringTime
		--Data.EngineProductionUnits
		--Data.EngineEngineeringCost
		--Data.EngineMaterialCost
		--Data.EngineToolingCosts

		--Data.TrimEngineeringTime
		--Data.TrimManHours
		--Data.TrimEngCosts
		--Data.MatCost
		--Data.ToolingCosts

	--Medium factory, cheap labor, medium automation, 2 shifts
	Data.EngineCostPreset0 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   200, 10, 1.5, 2)

	--Large factory, cheap labor, medium automation, 2 shifts
	Data.EngineCostPreset1 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   500, 10, 1.5, 2)

	--Medium factory, average labor, high automation, 2 shifts
	Data.EngineCostPreset2 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   150, 20, 2, 2)

	--Medium factory, average labor, high automation, 3 shifts
	Data.EngineCostPreset3 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
			   							   100, 20, 2, 3)

	--Small factory, average labor, medium automation, 2 shifts
	Data.EngineCostPreset4 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   50, 20, 1.5, 2)


	--Small factory, cheap labor, medium automation, 3 shifts
	Data.EngineCostPreset5 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   50, 10, 1.5, 3)

	--Tiny factory, cheap labor, no automation, 2 shifts
	Data.EngineCostPreset6 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   10, 10, 1, 2)

	--Tiny factory, average labor, no automation, 2 shifts
	Data.EngineCostPreset7 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
								    	   10, 20, 1, 2)

	--Tiny factory, expensive labor, no automation, 1 shift
	Data.EngineCostPreset8 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
										   10, 30, 1, 1)

	--Medium factory, very cheap labor, low automation, 3 shifts
	Data.EngineCostPreset9 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
									       200, 5, 1.2, 3)

	--Large factory, very cheap labor, low automation, 3 shifts
	Data.EngineCostPreset10 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
											500, 5, 1.2, 3)

	--Large factory, average labor, medium automation, 2 shifts
	Data.EngineCostPreset11 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
											500, 20, 1.5, 2)

	--Small factory, expensive labor, high automation, 2 shifts
	Data.EngineCostPreset12 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
											50, 30, 2, 2)

	--Tiny factory, expensive labor, high automation, 2 shifts
	Data.EngineCostPreset13 = CalculateCost(Data.EngineEngineeringTime, Data.EngineProductionUnits, Data.EngineEngineeringCost, Data.EngineMaterialCost, Data.EngineToolingCosts,
											10, 30, 2, 2)
	return Data
end


function CExporter.ExportCarFiles(CarCalculator)
	Files = { }
	return Files
end
