#include <string.h>
#include "raylib.h"

#include "research.h"
#include "sector.h"
#include "unit.h"

Unit_Design designs[DESIGN_MAX];
int designnum = 0;

Unit_Component Unit_allComponents[COMPONENTS_ALL] = {
    (Unit_Component) { // 0
        .name = "Warp Device I",
        .warpDriveLevel = 1,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_WARP_2
    },
    (Unit_Component) { // 1
        .name = "Habitation 1",
        .habitationSpace = 50,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_FABRICS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 2
        .name = "Storage 1",
        .storageCapacity = 1000,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 3
        .name = "Fabricator 1",
        .unitProductionVolume = 100,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_CONSTRUCT_1
    },
    (Unit_Component) { // 4
        .name="Miner 1",
        .miningVolume = 50,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_MINING_2
    },
    (Unit_Component) { // 5
        .name="Farm 1",
        .foodProduction = 25,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 1},
            {RES_FERTILE_SOIL, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 6
        .name="Research Lab 1",
        .researchVolume = {
            5, 5, 5, 5, 5, 5
        },
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 7
        .name="Laser Cannon",
        .shotPower = 10,
        .shotPenetration = 25,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_BEAM_2
    },
    (Unit_Component) { // 8
        .name="Shield 1",
        .shieldStrength = 10,
        .sheildRechargeRate = 1,
        .size = 250,
        .buildCosts = {
            {RES_BASE_METALS, 25},
            {RES_MAGNETRIUM, 25},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_SHIELD_2
    },
    (Unit_Component) { // 9
        .name="Gun Battery",
        .shotPower = 25,
        .shotPenetration = 10,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_GUN_2
    },
    (Unit_Component) {
        .name="Titanium Armour",
        .armourStrength = 100,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_ARMOUR_2
    },
    (Unit_Component) {
        .name="Carrier 1",
        .maxAllowedShipSize = 500,
        .totalShipSize = 3000,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .obsoletedBy = RT_CARRIER_1
    },
    // Warp components
    (Unit_Component) {
        .name = "Warp Device 2",
        .warpDriveLevel = 2,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_2,
        .obsoletedBy = RT_WARP_3
    },
    (Unit_Component) {
        .name = "Warp Device 3",
        .warpDriveLevel = 3,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_3,
        .obsoletedBy = RT_WARP_4
    },
    (Unit_Component) {
        .name = "Warp Device 4",
        .warpDriveLevel = 4,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_4,
        .obsoletedBy = RT_WARP_5
    },
    (Unit_Component) {
        .name = "Warp Device 5",
        .warpDriveLevel = 5,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_5,
        .obsoletedBy = RT_WARP_6
    },
    (Unit_Component) {
        .name = "Warp Device 6",
        .warpDriveLevel = 6,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_6,
        .obsoletedBy = RT_WARP_7
    },
    (Unit_Component) {
        .name = "Warp Device 7",
        .warpDriveLevel = 7,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_7,
        .obsoletedBy = RT_WARP_8
    },
    (Unit_Component) {
        .name = "Warp Device 8",
        .warpDriveLevel = 8,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_8,
        .obsoletedBy = RT_WARP_9
    },
    (Unit_Component) {
        .name = "Warp Device 9",
        .warpDriveLevel = 9,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_9,
        .obsoletedBy = RT_WARP_10
    },
    (Unit_Component) {
        .name = "Warp Device 10",
        .warpDriveLevel = 10,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_10
    },
    // Construction components
    (Unit_Component) {
        .name="Armour 2",
        .armourStrength = 200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 75},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_2,
        .obsoletedBy = RT_ARMOUR_3
    },
    (Unit_Component) {
        .name="Armour 3",
        .armourStrength = 400,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_3,
        .obsoletedBy = RT_ARMOUR_4
    },
    (Unit_Component) {
        .name="Armour 4",
        .armourStrength = 800,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_4,
        .obsoletedBy = RT_ARMOUR_5
    },
    (Unit_Component) {
        .name="Armour 5",
        .armourStrength = 1600,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_5,
        .obsoletedBy = RT_ARMOUR_6
    },
    (Unit_Component) {
        .name="Armour 6",
        .armourStrength = 3200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_6
    },
    (Unit_Component) {
        .name="Miner 2",
        .miningVolume = 50,
        .size = 750,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_2,
        .obsoletedBy = RT_MINING_3
    },
    (Unit_Component) {
        .name="Miner 3",
        .miningVolume = 100,
        .size = 1000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_3,
        .obsoletedBy = RT_MINING_4
    },
    (Unit_Component) {
        .name="Miner 4",
        .miningVolume = 200,
        .size = 1500,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_4,
        .obsoletedBy = RT_MINING_5
    },
    (Unit_Component) {
        .name="Miner 5",
        .miningVolume = 400,
        .size = 2000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_5,
        .obsoletedBy = RT_MINING_6
    },
    (Unit_Component) {
        .name="Miner 6",
        .miningVolume = 800,
        .size = 3000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_6
    },
    (Unit_Component) {
        .name="Carrier 2",
        .maxAllowedShipSize = 1000,
        .totalShipSize = 6000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_1,
        .obsoletedBy = RT_CARRIER_2
    },
    (Unit_Component) {
        .name="Carrier 3",
        .maxAllowedShipSize = 1000,
        .totalShipSize = 6000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_2,
        .obsoletedBy = RT_CARRIER_3
    },
    (Unit_Component) {
        .name="Carrier 4",
        .maxAllowedShipSize = 2000,
        .totalShipSize = 12000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_3,
        .obsoletedBy = RT_CARRIER_4
    },
    (Unit_Component) {
        .name="Carrier 5",
        .maxAllowedShipSize = 4000,
        .totalShipSize = 24000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_4,
        .obsoletedBy = RT_CARRIER_5
    },
    (Unit_Component) {
        .name="Carrier 6",
        .maxAllowedShipSize = 8000,
        .totalShipSize = 48000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_5
    },
    (Unit_Component) {
        .name = "Fabricator 2",
        .unitProductionVolume = 100,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_1,
        .obsoletedBy = RT_CONSTRUCT_2
    },
    (Unit_Component) {
        .name = "Fabricator 3",
        .unitProductionVolume = 200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_2,
        .obsoletedBy = RT_CONSTRUCT_3
    },
    (Unit_Component) {
        .name = "Fabricator 4",
        .unitProductionVolume = 400,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_3,
        .obsoletedBy = RT_CONSTRUCT_4
    },
    (Unit_Component) {
        .name = "Fabricator 5",
        .unitProductionVolume = 1600,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_4,
        .obsoletedBy = RT_CONSTRUCT_5
    },
    (Unit_Component) {
        .name = "Fabricator 6",
        .unitProductionVolume = 3200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_5
    },
    // Weapons tech
    (Unit_Component) {
        .name="Multilaster",
        .shotPower = 20,
        .shotPenetration = 50,
        .size = 50,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_2,
        .obsoletedBy = RT_BEAM_3
    },
    (Unit_Component) {
        .name="Ion Cannon",
        .shotPower = 40,
        .shotPenetration = 100,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 20},
            {RES_CYTRONIUM, 5},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_3,
        .obsoletedBy = RT_BEAM_4
    },
    (Unit_Component) {
        .name="Proton Blaster",
        .shotPower = 80,
        .shotPenetration = 200,
        .size = 200,
        .buildCosts = {
            {RES_BASE_METALS, 40},
            {RES_CYTRONIUM, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_4,
        .obsoletedBy = RT_BEAM_5
    },
    (Unit_Component) {
        .name="Plasma Cannon",
        .shotPower = 160,
        .shotPenetration = 400,
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 80},
            {RES_CYTRONIUM, 20},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_5,
        .obsoletedBy = RT_BEAM_6
    },
    (Unit_Component) {
        .name="Turbolaser",
        .shotPower = 320,
        .shotPenetration = 800,
        .size = 800,
        .buildCosts = {
            {RES_BASE_METALS, 160},
            {RES_CYTRONIUM, 40},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_6,
        .obsoletedBy = RT_BEAM_7
    },
    (Unit_Component) {
        .name="Tachyon Generator",
        .shotPower = 640,
        .shotPenetration = 1600,
        .size = 1600,
        .buildCosts = {
            {RES_BASE_METALS, 320},
            {RES_CYTRONIUM, 80},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_7,
        .obsoletedBy = RT_BEAM_8
    },
    (Unit_Component) {
        .name="Hyperbeam Emitter",
        .shotPower = 1280,
        .shotPenetration = 3200,
        .size = 3200,
        .buildCosts = {
            {RES_BASE_METALS, 640},
            {RES_CYTRONIUM, 160},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_BEAM_8
    },
    (Unit_Component) {
        .name="Mass Driver",
        .shotPower = 50,
        .shotPenetration = 20,
        .size = 50,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_GUN_2,
        .obsoletedBy = RT_GUN_3
    },
    (Unit_Component) {
        .name="Gauss Cannon",
        .shotPower = 100,
        .shotPenetration = 40,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 20},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_GUN_3,
        .obsoletedBy = RT_GUN_4
    },
    (Unit_Component) {
        .name="Macrodriver",
        .shotPower = 300,
        .shotPenetration = 120,
        .size = 200,
        .buildCosts = {
            {RES_BASE_METALS, 60},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_GUN_4,
        .obsoletedBy = RT_GUN_5
    },
    (Unit_Component) {
        .name="Assault Cannon",
        .shotPower = 800,
        .shotPenetration = 320,
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 160},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_GUN_5,
        .obsoletedBy = RT_GUN_6
    },
    (Unit_Component) {
        .name="Planetoid Launcher",
        .shotPower = 1600,
        .shotPenetration = 640,
        .size = 800,
        .buildCosts = {
            {RES_BASE_METALS, 320},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_GUN_6
    },
    (Unit_Component) {
        .name="Fusion Torpedo",
        .shotPower = 60,
        .shotPenetration = 60,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_MAGNETRIUM, 5},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_TORPEDO_1,
        .obsoletedBy = RT_TORPEDO_2
    },
    (Unit_Component) {
        .name="Proton Torpedo",
        .shotPower = 240,
        .shotPenetration = 240,
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 40},
            {RES_MAGNETRIUM, 20},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_TORPEDO_2,
        .obsoletedBy = RT_TORPEDO_3
    },
    (Unit_Component) {
        .name="Antimatter Torpedo",
        .shotPower = 720,
        .shotPenetration = 720,
        .size = 1200,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_MAGNETRIUM, 60},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_TORPEDO_3,
        .obsoletedBy = RT_TORPEDO_4
    },
    (Unit_Component) {
        .name="Singularity Torpedo",
        .shotPower = 1440,
        .shotPenetration = 1440,
        .size = 2400,
        .buildCosts = {
            {RES_BASE_METALS, 240},
            {RES_MAGNETRIUM, 120},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_TORPEDO_4
    },
    // Espionage techs
    (Unit_Component) {
        .name = "Antenna 1",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ANTENNA_1,
        .obsoletedBy = RT_ANTENNA_2
    },
    (Unit_Component) {
        .name = "Antenna 2",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ANTENNA_2,
        .obsoletedBy = RT_ANTENNA_3
    },
    (Unit_Component) {
        .name = "Antenna 3",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ANTENNA_3,
        .obsoletedBy = RT_ANTENNA_4
    },
    (Unit_Component) {
        .name = "Antenna 4",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ANTENNA_4,
        .obsoletedBy = RT_ANTENNA_5
    },
    (Unit_Component) {
        .name = "Antenna 5",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ANTENNA_5
    },
    (Unit_Component) {
        .name = "Encryptor 1",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ENCRYPTOR_1,
        .obsoletedBy = RT_ENCRYPTOR_2
    },
    (Unit_Component) {
        .name = "Encryptor 2",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ENCRYPTOR_2,
        .obsoletedBy = RT_ENCRYPTOR_3
    },
    (Unit_Component) {
        .name = "Encryptor 3",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ENCRYPTOR_3,
        .obsoletedBy = RT_ENCRYPTOR_4
    },
    (Unit_Component) {
        .name = "Encryptor 4",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ENCRYPTOR_4,
        .obsoletedBy = RT_ENCRYPTOR_5
    },
    (Unit_Component) {
        .name = "Encryptor 5",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_ENCRYPTOR_5
    },
    (Unit_Component) {
        .name = "Override 1",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_OVERRIDE_1,
        .obsoletedBy = RT_OVERRIDE_2
    },
    (Unit_Component) {
        .name = "Override 2",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_OVERRIDE_2,
        .obsoletedBy = RT_OVERRIDE_3
    },
    (Unit_Component) {
        .name = "Override 3",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_OVERRIDE_3,
        .obsoletedBy = RT_OVERRIDE_4
    },
    (Unit_Component) {
        .name = "Override 4",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_OVERRIDE_4,
        .obsoletedBy = RT_OVERRIDE_5
    },
    (Unit_Component) {
        .name = "Override 5",
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_OVERRIDE_5
    },
    // Luxuries
    (Unit_Component) {
        .name = "Luxury Goods 1",
        .goodProduced = RES_LUXURY_1,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_GOODS_1,
        .obsoletedBy = RT_GOODS_2
    },
    (Unit_Component) {
        .name = "Luxury Goods 2",
        .goodProduced = RES_LUXURY_2,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_GOODS_2,
        .obsoletedBy = RT_GOODS_3
    },
    (Unit_Component) {
        .name = "Luxury Goods 3",
        .goodProduced = RES_LUXURY_3,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_GOODS_3,
        .obsoletedBy = RT_GOODS_4
    },
    (Unit_Component) {
        .name = "Luxury Goods 4",
        .goodProduced = RES_LUXURY_4,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_GOODS_4,
        .obsoletedBy = RT_GOODS_5
    },
    (Unit_Component) {
        .name = "Luxury Goods 5",
        .goodProduced = RES_LUXURY_5,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_GOODS_5
    },
    (Unit_Component) {
        .name = "Barathian Wine 1",
        .goodProduced = RES_WINE_1,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_WINE_1,
        .obsoletedBy = RT_WINE_2
    },
    (Unit_Component) {
        .name = "Barathian Wine 2",
        .goodProduced = RES_WINE_2,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_WINE_2,
        .obsoletedBy = RT_WINE_3
    },
    (Unit_Component) {
        .name = "Barathian Wine 3",
        .goodProduced = RES_WINE_3,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_WINE_3,
        .obsoletedBy = RT_WINE_4
    },
    (Unit_Component) {
        .name = "Barathian Wine 4",
        .goodProduced = RES_WINE_4,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_WINE_4,
        .obsoletedBy = RT_WINE_5
    },
    (Unit_Component) {
        .name = "Barathian Wine 5",
        .goodProduced = RES_WINE_5,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_WINE_5
    },
    (Unit_Component) {
        .name = "Medical Supplies 1",
        .goodProduced = RES_MEDICAL_1,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_MEDICAL_1,
        .obsoletedBy = RT_MEDICAL_2
    },
    (Unit_Component) {
        .name = "Medical Supplies 2",
        .goodProduced = RES_MEDICAL_2,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_MEDICAL_2,
        .obsoletedBy = RT_MEDICAL_3
    },
    (Unit_Component) {
        .name = "Medical Supplies 3",
        .goodProduced = RES_MEDICAL_3,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_MEDICAL_3,
        .obsoletedBy = RT_MEDICAL_4
    },
    (Unit_Component) {
        .name = "Medical Supplies 4",
        .goodProduced = RES_MEDICAL_4,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_MEDICAL_4,
        .obsoletedBy = RT_MEDICAL_5
    },
    (Unit_Component) {
        .name = "Medical Supplies 5",
        .goodProduced = RES_MEDICAL_5,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_MEDICAL_5
    },
    (Unit_Component) {
        .name = "Stims 1",
        .goodProduced = RES_STIM_1,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_STIMS_1,
        .obsoletedBy = RT_STIMS_2
    },
    (Unit_Component) {
        .name = "Stims 2",
        .goodProduced = RES_STIM_2,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_STIMS_2,
        .obsoletedBy = RT_STIMS_3
    },
    (Unit_Component) {
        .name = "Stims 3",
        .goodProduced = RES_STIM_3,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_STIMS_3,
        .obsoletedBy = RT_STIMS_4
    },
    (Unit_Component) {
        .name = "Stims 4",
        .goodProduced = RES_STIM_4,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_STIMS_4,
        .obsoletedBy = RT_STIMS_5
    },
    (Unit_Component) {
        .name = "Stims 5",
        .goodProduced = RES_STIM_5,
        .buildCosts = {
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0},
        },
        .techRequired = RT_STIMS_5
    },
    // Forcefield tech
    (Unit_Component) {
        .name="Shield 2",
        .shieldStrength = 20,
        .sheildRechargeRate = 2,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_MAGNETRIUM, 50},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_SHIELD_2,
        .obsoletedBy = RT_SHIELD_3
    },
    (Unit_Component) {
        .name="Shield 3",
        .shieldStrength = 40,
        .sheildRechargeRate = 4,
        .size = 200,
        .buildCosts = {
            {RES_BASE_METALS, 100},
            {RES_MAGNETRIUM, 100},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_SHIELD_3,
        .obsoletedBy = RT_SHIELD_4
    },
    (Unit_Component) {
        .name="Shield 4",
        .shieldStrength = 80,
        .sheildRechargeRate = 8,
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 200},
            {RES_MAGNETRIUM, 200},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_SHIELD_4,
        .obsoletedBy = RT_SHIELD_5
    },
    (Unit_Component) {
        .name="Shield 5",
        .shieldStrength = 160,
        .sheildRechargeRate = 16,
        .size = 800,
        .buildCosts = {
            {RES_BASE_METALS, 400},
            {RES_MAGNETRIUM, 400},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_SHIELD_5,
        .obsoletedBy = RT_SHIELD_6
    },
    (Unit_Component) {
        .name="Shield 6",
        .shieldStrength = 320,
        .sheildRechargeRate = 32,
        .size = 1600,
        .buildCosts = {
            {RES_BASE_METALS, 800},
            {RES_MAGNETRIUM, 800},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_SHIELD_6
    },
    (Unit_Component) {
        .name="Reflector Sheilds 1",
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_MAGNETRIUM, 20},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_REFLECTOR_1,
        .obsoletedBy = RT_REFLECTOR_2
    },
    (Unit_Component) {
        .name="Reflector Sheilds 2",
        .size = 200,
        .buildCosts = {
            {RES_BASE_METALS, 20},
            {RES_MAGNETRIUM, 40},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_REFLECTOR_2,
        .obsoletedBy = RT_REFLECTOR_3
    },
    (Unit_Component) {
        .name="Reflector Sheilds 3",
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 40},
            {RES_MAGNETRIUM, 80},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_REFLECTOR_3,
        .obsoletedBy = RT_REFLECTOR_4
    },
    (Unit_Component) {
        .name="Reflector Sheilds 4",
        .size = 800,
        .buildCosts = {
            {RES_BASE_METALS, 80},
            {RES_MAGNETRIUM, 160},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_REFLECTOR_4,
        .obsoletedBy = RT_REFLECTOR_5
    },
    (Unit_Component) {
        .name="Reflector Sheilds 5",
        .size = 1600,
        .buildCosts = {
            {RES_BASE_METALS, 160},
            {RES_MAGNETRIUM, 320},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_REFLECTOR_5
    },
    (Unit_Component) {
        .name="Charger 1",
        .size = 100,
        .sheildRechargeRate = 10,
        .buildCosts = {
            {RES_BASE_METALS, 30},
            {RES_DEUTERIUM, 5},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CHARGER_1,
        .obsoletedBy = RT_CHARGER_2
    },
    (Unit_Component) {
        .name="Charger 2",
        .size = 200,
        .sheildRechargeRate = 20,
        .buildCosts = {
            {RES_BASE_METALS, 60},
            {RES_DEUTERIUM, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CHARGER_2,
        .obsoletedBy = RT_CHARGER_3
    },
    (Unit_Component) {
        .name="Charger 3",
        .size = 400,
        .sheildRechargeRate = 40,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_DEUTERIUM, 20},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CHARGER_3,
        .obsoletedBy = RT_CHARGER_4
    },
    (Unit_Component) {
        .name="Charger 4",
        .size = 800,
        .sheildRechargeRate = 80,
        .buildCosts = {
            {RES_BASE_METALS, 240},
            {RES_DEUTERIUM, 40},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CHARGER_4,
        .obsoletedBy = RT_CHARGER_5
    },
    (Unit_Component) {
        .name="Charger 5",
        .size = 1600,
        .sheildRechargeRate = 160,
        .buildCosts = {
            {RES_BASE_METALS, 480},
            {RES_DEUTERIUM, 80},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CHARGER_5
    },
    (Unit_Component) {
        .name="Drainer 1",
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_MAGNETRIUM, 50},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_DRAINER_1,
        .obsoletedBy = RT_DRAINER_2
    },
    (Unit_Component) {
        .name="Drainer 2",
        .size = 200,
        .buildCosts = {
            {RES_BASE_METALS, 20},
            {RES_MAGNETRIUM, 100},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_DRAINER_2,
        .obsoletedBy = RT_DRAINER_3
    },
    (Unit_Component) {
        .name="Drainer 3",
        .size = 400,
        .buildCosts = {
            {RES_BASE_METALS, 40},
            {RES_MAGNETRIUM, 200},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_DRAINER_3,
        .obsoletedBy = RT_DRAINER_4
    },
    (Unit_Component) {
        .name="Drainer 4",
        .size = 800,
        .buildCosts = {
            {RES_BASE_METALS, 80},
            {RES_MAGNETRIUM, 400},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_DRAINER_4,
        .obsoletedBy = RT_DRAINER_5
    },
    (Unit_Component) {
        .name="Drainer 5",
        .size = 1600,
        .buildCosts = {
            {RES_BASE_METALS, 160},
            {RES_MAGNETRIUM, 700},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_DRAINER_5
    }
};

Unit_Component Unit_getComponent(int index) {
    return Unit_allComponents[index];
}

Unit_Design* Unit_createDesign() {
    Unit_Design* d = &designs[designnum];
    d->id = designnum;
    d->shipSize = 1;
    for (int i = 0; i < UNIT_COMPONENT_MAX; i++) {
        d->components[i] = (Unit_Component) {};
    }
    d->componentnum = 0;
    designnum++;
    return d;
}

int Unit_designProductionCost(Unit_Design* d) {
    int pc = 0;
    for (int i = 0; i < d->componentnum; i++) {
        pc += d->components[i].productionCost;
    }
    return pc;
}

int Unit_designCount() {
    return designnum;
}

Unit_Design Unit_getDesignCopy(int id) {
    return designs[id];
}

Unit_Design* Unit_getDesignPointer(int id) {
    return &designs[id];
}

Unit_Entity units[1024];
int unitnum = 0;
int addIndex = 0;

Unit_Entity Unit_generate(Unit_Design* design) {
    Unit_Entity u = (Unit_Entity) {};
    strcpy(u.name, TextFormat("%s %d", design->name, addIndex));
    u.hpMax = 100;
    Unit_Component* c = 0;
    int numweapons = 0;
    for (int i = 0; i < design->componentnum; i++) {
        u.pop++;
        c = &design->components[i];
        u.hpMax += c->armourStrength;
        u.shieldMax += c->shieldStrength;
        if (c->warpDriveLevel > u.warpDriveLevel) {
            u.warpDriveLevel = c->warpDriveLevel;
        }
        u.storage += c->storageCapacity;
        u.popMax += c->habitationSpace;
        u.mining += c->miningVolume;
        u.farming += c->foodProduction;
        u.production += c->unitProductionVolume;
        for (int j = 0; j < FIELD_ALL; j++) {
            u.research[j] += c->researchVolume[j];
        }
        // if it's a weapon put the component into the weapons slot
        if (c->shotPower || c->shotPenetration) {
            u.weapons[numweapons] = *c;
            numweapons++;
            u.canFight = 1;
        }
        // Calculate production cost
        for (int j = 0; j < 4; j++) {
            u.costToBuild += c->buildCosts[j].abundance * 100;
        }
        u.size += c->size;
        u.totalShipStorage += c->totalShipSize;
        if (c->maxAllowedShipSize > u.allowsShipSize) {
            u.allowsShipSize = c->maxAllowedShipSize;
        }
    }
    u.hp = u.hpMax;
    u.shields = u.shieldMax;
    // Not sure what we're doing about this
    u.design = design->id;
    return u;
}

Unit_Entity* Unit_create(Unit_Design* design) {
    addIndex = unitnum;
    for (int i = 0; i < unitnum; i++) {
        if (units[i].hp <= 0) {
            addIndex = i;
            unitnum--;
            break;
        }
    }
    units[addIndex] = Unit_generate(design);
    Unit_Entity* u = &units[addIndex];
    unitnum++;
    return u;
}

Unit_Entity Unit_getCopy(int uid) {
    return units[uid];
}

Unit_Entity* Unit_getPointer(int uid) {
    return &units[uid];
}

int Unit_count() {
    return unitnum;
}

int Unit_lastAddedIndex() {
    return addIndex;
}

void Unit_storeItem(Unit_Entity* u, Unit_Inventory inv) {
    u->totalStored += inv.quantity;
    for (int i = 0; i < u->storednum; i++) {
        Unit_Inventory* existingInv = &u->stored[i];
        if (existingInv->storedResourceId == inv.storedResourceId) {
            existingInv->quantity += inv.quantity;
            return;
        }
    }
    u->stored[u->storednum] = inv;
    u->storednum++;
}

int Unit_consumeItem(Unit_Entity* u, Sector_Resource r) {
    if (u->storednum == 0) {
        return 0;
    }
    for (int i = 0; i < u->storednum; i++) {
        Unit_Inventory* existingInv = &u->stored[i];
        if (existingInv->storedResourceId == r.type) {
            if (existingInv->quantity >= r.abundance) {
                existingInv->quantity -= r.abundance;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

int Unit_getTotalStored(Unit_Entity* u, Sector_Resource r) {
    if (u->storednum == 0) {
        return 0;
    }
    for (int i = 0; i < u->storednum; i++) {
        Unit_Inventory* existingInv = &u->stored[i];
        if (existingInv->storedResourceId == r.type) {
            return existingInv->quantity;
        }
    }
    return 0;
}

void Unit_inventoryTransfer(Unit_Entity* from, int invIndex, Unit_Entity* to) {
    Unit_storeItem(to, from->stored[invIndex]);
    Unit_removeItemByIndex(from, invIndex);
}

void Unit_removeItemByIndex(Unit_Entity* u, int index) {
    u->totalStored -= u->stored[index].quantity;
    for (int i = index; i < u->storednum; i++) {
        u->stored[i] = u->stored[i + 1];
    }
    u->storednum--;
}
