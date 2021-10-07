const page pages[] = {
    {"Start here", "This is an \001in progress\002 version of the\nPlayer's Handbook for the TI-84+ CE calc.\nHope you like it :-)\n\nTo use this copy, simply choose the topic\nfrom the menu list. Use <- and -> to navigate\nthrough pages and the number keys to choose.\n\nHave fun!\n"},
    {"-- Classes", "This section contains information about\ndifferent classes.\n"},
    {"Barbarian 1", "\001Hit points\002\n5+LVL*(7+CON)\n\001Proficiencies\002\nLight/Med armor, shield, all weapons\n\001Saving throws\002 STR, CON\n\n\001Unarmored Defense\002\nAC=10+DEX+CON\n\001Rage\002\nCan enter as B.A. When no heavy armor:\n- Adv. on STR checks/saves\n- Get +(L-8)2/(L-15)3/(L-20)4 dmg. bonus\n- Resis. to bludgeoning/piercing/slashing\nEnds if 1 min./unconscious/no atk or dmg\nUse (L-2)2/(L-5)3/(L-11)4/(L-16)5/(L-19)6\ntimes per long rest\n\001Danger Sense (2+)\002\nAdv. on DEX saves against visible effects\n"},
    {"Barbarian 2", "\001Extra Attack (5+)\002\nCan attack twice/round\n\001Fast Movement (5+)\002\nSpeed+10 when no heavy armor\n\001Feral Instinct (7+)\002\nAdv. on initiatie rolls and can act when\nsurprised if enter rage as first thing\n"},
    {"Bard 1", "\001Hit points\002\n3+LVL*(5+CON)\n\001Proficiencies\002\nLight armor, simp. weapons, hand crossbow,\n\001longsword\002, rapier, shortsword, 3 instruments\n\001Saving throws\002 DEX, CHA\n\n\001Spellcasting\002\n\001Spellcasting ability\002 CHA\n\001Cantrips\002 (L-3)2/(L-9)3/(L-20)4\n\001Learn\002 1/lvl, 2@L10,14,18, 0@L12,16,19-\n(Can also replace 1/lvl)\n\001Bardic Inspiration\002\nCHA times/L.R., 1 creature within 60ft get die\nD(L-4)6/(L-9)8/(L-14)10/(L-20)12, use on\ncheck/attack/save\n\001Jack of All Trades (2+)\002\nAdd prof. bonus/2 (down) to not prof. checks\n\001Expertise (3,10)\002\nChoose two prof. skills, add 2x prof. bonus\n"},
    {"Bard 2", "\001Font of Inspiration (5+)\002\nRegain Inspiration uses after S.R.\n\001Countercharm (6+)\002\nAs action, start perform that lasts 1 round.\nYou + friendly in 30ft adv. on saves against\nbeing frightened/charmed.\n"},
    {"", "Empty\n"},
    {"", "Empty\n"},
    {"-- Weapons", "This section contains information about\nthe weapons you can find.\n"},
    {"Properties", "\001All distances are divided by 10.\002\n\001(A)mmunition\002 Can find back 1/2 (short/long)\n\001(F)inesse\002 Can use STR/DEX\n\001(H)eavy\002 Small creatures have disadv.\n\001(L)ight\002 Two-weapon rule, see later\n\001L(o)ading\002 Only one attack with this\n\001(R)each\002 Reach +5ft\n\001(T)hrown\002 (short/long)\n\001T(w)o-Handed\002 Self-explanatory\n\001(V)ersatile\002 One- or two-handed (2-hand dmg)\n\001*\002 Special: see Special Weapons section.\n"},
    {"Simple Melee Weapons", "\001Club\002, 1sp, 1d4 bludgeoning, L\n\001Dagger\002, 2gp, 1d4 piercing, FLT(2/6)\n\001Greatclub\002, 2sp, 1d8 bludgeoning, W\n\001Handaxe\002, 5gp, 1d6 slashing, LT(2/6)\n\001Javelin\002, 5sp, 1d6 piercing, T(3/12)\n\001Light hammer\002, 2gp, 1d4 bludgeoning, LT(2/6)\n\001Mace\002, 5gp, 1d6 bludgeoning\n\001Quarterstaff\002, 2sp, 1d6 bludgeoning, V(8)\n\001Sickle\002, 1gp, 1d4 slashing, L\n\001Spear\002, 1gp, 1d6 piercing, T(2/6)V(8)\n"},
    {"Simple Ranged Weapons", "\001Light crossbow\002, 25gp, 1d8 piercing, A(8/32)LW\n\001Dart\002, 5cp, 1d4 piercing, FT(2/6)\n\001Shortbow\002, 25gp, 1d6 piercing, A(8/32)W\n\001Sling\002, 1sp, 1d4 bludgeoning, A(3/12)\n"},
    {"Martial Melee Weapons", "\001Battleaxe\002, 10gp, 1d8 slashing, V(10)\n\001Flail\002, 10gp, 1d8 bludgeoning\n\001Glaive\002, 20gp, 1d10 slashing, HRW\n\001Greataxe\002, 30gp, 1d12 slashing, HW\n\001Greatsword\002, 50gp, 2d6 slashing, HW\n\001Halberd\002, 20gp, 1d10 slashing, HRW\n\001Lance\002, 10gp, 1d12 piercing, R*\n\001Longsword\002, 15gp, 1d8 slashing, V(10)\n\001Maul\002, 10gp, 2d6 bludgeoning, HW\n\001Morningstar\002, 15gp, 1d8 piercing\n\001Pike\002, 5gp, 1d10 piercing, HRW\n\001Rapier\002, 25gp, 1d8 piercing, F\n\001Scimitar\002, 25gp, 1d6 slashing, FL\n\001Shortsword\002, 10gp, 1d6 piercing, FL\n\001Trident\002, 5gp, 1d6 piercing, T(2/6)V(8)\n\001War pick\002, 5gp, 1d8 piercing\n\001Warhammer\002, 15gp, 1d8 bludgeoning, V(10)\n\001Whip\002, 2gp, 1d4 slashing, FR\n"},
    {"Martial Ranged Weapons", "\001Blowgun\002, 10gp, 1 piercing, A(2.5/10)O\n\001Hand crossbow\002, 75gp, 1d6 piercing, A(3/12)LO\n\001Heavy crossbow\002, 50gp, 1d10 piercing, \nA(10/40)HOW\n\001Longbow\002, 50gp, 1d8 piercing, A(15/60)HW\n\001Net\002, 1gp, T(0.5/1.5)*\n"},
    {"Special Weapons", "\001Lance\002 Disadv. when attack within 5ft.\nNeed two hands if not mounted.\n\n\001Net\002 Large- creature restrained, can use\naction to make DC10 STR check to free, 5\nslashing damage destroys the net.\n"},
    {"", "Empty\n"},
    {"-- Misc. Rules", "This section contains information about\nsome miscellaneous rules in the PHB.\n"},
    {"Armor", "\001Light armor\002\n\001Padded\002, 5gp, 11+DEX, Disadv (Stealth), 8lb\n\001Leather\002, 10gp, 11+DEX, 10lb\n\001Studded leather\002, 45gp, 12+DEX, 13lb\n\n\001Medium Armor\002\n\001Hide\002, 10gp, 12+min(DEX,2), 12lb\n\001Chain shirt\002, 50gp, 13+min(DEX,2), 20lb\n\001Scale mail\002, 50gp, 14+min(DEX,2), Disadv, 45lb\n\001Breastplate\002, 400gp, 14+min(DEX,2), 20lb\n\001Half plate\002, 750gp, 15+min(DEX,2), Disadv, 20lb\n\n\001Heavy Armor\002\n\001Ring mail\002, 30gp, 14, Disadv, 40lb\n\001Chain mail\002, 75gp, 16, STR 13, Disadv, 55lb\n\001Splint\002, 200gp, 17, STR 15, Disadv, 60lb\n\001Plate\002, 1500gp, 18, STR 15, Disadv, 65lb\n\n\001Shield\002\n\001Shield\002, 10gp, +2, 6lb\n"},
    {"Equipment Packs P1", "\001Burglar's Pack (16gp)\002 Backpack, 1000 ball\nbearings, 10ft string, bell, 5 candles,\ncrowbar, hammer, 10 pitons, hooded lantern,\n2 flasks of oil, 5 days rations, tinderbox,\nwaterskin, 50ft hempen rope\n\001Diplomat's Pack (39gp)\002 Chest, 2 map/scroll\ncases, fine clothes, bottle of ink, ink pen,\nlamp, 2 flasks of oil, 5 paper, vial of perfume,\nsealing wax, soap\n\001Dungeoneer's Pack (12gp)\002 Backpack, crowbar,\nhammer, 10 pitons, 10 torches, tinderbox, 10\ndays rations, waterskin, 50ft hempen rope\n\001Entertainer's Pack (40gp)\002 Backpack, bedroll,\n2 costumes, 5 candles, 5 days rations,\nwaterskin, disguise kit\n\001Priest's Pack (19gp)\002 Backpack, blanket, 10\ncandles, tinderbox, alms box, 2 blocks\nincense, censer, vestments, 2 days rations,\nwaterskin\n"},
    {"Equipment Packs P2", "\001Scholar's Pack (40gp)\002 Backpack, book of lore,\nbottle of ink, ink pen, 10 parchment, little bag\nof sand, small knife\n"},
    {"-- Spell Lists", "This section contains information about\nthe spell lists of each class.\n"},
    {"Bard P1", "\001Cantrips (0 Level)\002\nDancing Lights\nFriends\nLight\nMage Hand\nMending\nMessage\nMinor Illusion\nPrestidigitation\nTrue Strike\nVicious Mockery\n\0011st Level\002\nAnimal Friendship\nBane\nCharm Person\nComprehend Languages\nCure Wounds\nDetect Magic\nDisguise Self\nDissonant Whispers\n"},
    {"Bard P2", "\0011st Level (continued)\002\nFaerie Fire\nFeather Fall\nHealing Word\nHeroism\nIdentify\nIllusory Script\nLongstrider\nSilent Image\nSleep\nSpeak with Animals\nTasha's Hideous Laughter\nThunderwave\nUnseen Servant\n\0012nd Level\002\nAnimal Messenger\nBlindness/Deafness\nCalm Emotions\nCloud of Daggers\nCrown of Madness\n"},
    {"Bard P3", "\0012nd Level (continued)\002\nDetect Thoughts\nEnhance Ability\nEnthrall\nHeat Metal\nHold Person\nInvisibility\nKnock\nLesser Restoration\nLocate Animals or Plants\nLocate Object\nMagic Mouth\nPhantasmal Force\nSee Invisibility\nShatter\nSilence\nSuggestion\nZone of Truth\n"},
    {"Bard P4", "\0013rd Level\002\nBestow Curse\nClairvoyance\nDispel Magic\nFear\nFeign Death\nGlyph of Warding\nHypnotic Pattern\nLeomund's Tiny Hut\nMajor Image\nNondetection\nPlant Growth\nSending\nSpeak with Dead\nSpeak with Plants\nStinking Cloud\nTongues\n"},
    {"Bard P5", "\0014th Level\002\nCompulsion\nConfusion\nDimension Door\nFreedom of Movement\nGreater Invisibility\nHallucinatory Terrain\nLocate Creature\nPolymorph\n\0015th Level\002\nAnimate Objects\nAwaken\nDominate Person\nDream\nGeas\nGreater Restoration\nHold Monster\nLegend Lore\nMass Cure Wounds\nMislead\n"},
    {"Bard P6", "\0015th Level (continued)\002\nModify Memory\nPlanar Binding\nRaise Dead\nScrying\nSeeming\nTeleportation Circle\n\0016th Level\002\nEyebite\nFind the Path\nGuards and Wards\nMass Suggestion\nOtto's Irresistible Dance\nProgrammed Illusion\nTrue Seeing\n"},
    {"Bard P7", "\0017th Level\002\nEtherealness\nForcecage\nMirage Arcane\nMordenkainen's Magnificent Mansion\nMordenkainen's Sword\nProject Image\nRegenerate\nResurrection\nSymbol\nTeleport\n\0018th Level\002\nDominate Monster\nFeeblemind\nGlibness\nMind Blank\nPower Word Stun\n"},
    {"Bard P8", "\0019th Level\002\nForesight\nPower Word Heal\nPower Word Kill\nTrue Polymorph\n"},
    {"Cleric P1", "\001Cantrips (0 Level)\002\nGuidance\nLight\nMending\nResistance\nSacred Flame\nSpare the Dying\nThaumaturgy\n\0011st Level\002\nBane\nBless\nCommand\nCreate or Destroy Water\nCure Wounds\nDetect Evil and Good\nDetect Magic\nDetect Poison and Disease\nGuiding Bolt\nHealing Word\nInflict Wounds\n"},
    {"Cleric P2", "\0011st Level (continued)\002\nProtection from Evil and Good\nPurify Food and Drink\nSanctuary\nShield of Faith\n\0012nd Level\002\nAid\nAugury\nBlindness/Deafness\nCalm Emotions\nContinual Flame\nEnhance Ability\nFind Traps\nGentle Repose\nHold Person\nLesser Restoration\nLocate Object\nPrayer of Healing\nProtection from Poison\nSilence\n"},
    {"Cleric P3", "\0012nd Level (continued)\002\nSpiritual Weapon\nWarding Bond\nZone of Truth\n\0013rd Level\002\nAnimate Dead\nBeacon of Hope\nBestow Curse\nClairvoyance\nCreate Food and Water\nDaylight\nDispel Magic\nFeign Death\nGlyph of Warding\nMagic Circle\nMass Healing Word\nMeld into Stone\nProtection from Energy\nRemove Curse\nRevivify\n"},
    {"Cleric P4", "\0013rd Level (continued)\002\nSending\nSpeak with Dead\nSpirit Guardians\nTongues\nWater Walk\n\0014th Level\002\nBanishment\nControl Water\nDeath Ward\nDivination\nFreedom of Movement\nGuardian of Faith\nLocate Creature\nStone Shape\n\0015th Level\002\nCommune\nContagion\nDispel Evil and Good\nFlame Strike\n"},
    {"Cleric P5", "\0015th Level (continued)\002\nGeas\nGreater Restoration\nHallow\nInsect Plague\nLegend Lore\nMass Cure Wounds\nPlanar Binding\nRaise Dead\nScrying\n\0016th Level\002\nBlade Barrier\nCreate Undead\nFind the Path\nForbiddance\nHarm\nHeal\nHeroes' Feast\nPlanar Ally\nTrue Seeing\n"},
    {"Cleric P6", "Word of Recall\n\0017th Level\002\nConjure Celestial\nDivine Word\nEtherealness\nFire Storm\nPlane Shift\nRegenerate\nResurrection\nSymbol\n\0018th Level\002\nAntimagic Field\nControl Weather\nEarthquake\nHoly Aura\n\0019th Level\002\nAstral Projection\nGate\nMass Heal\nTrue Resurrection\n"},
    {"Druid P1", "\001Cantrips (0 level)\002\nDruidcraft\nGuidance\nMending\nPoison Spray\nProduce Flame\nResistance\nShillelagh\nThorn Whip\n\0011st Level\002\nAnimal Friendship\nCharm Person\nCreate or Destroy Water\nCure Wounds\nDetect Magic\nDetect Poison and Disease\nEntangle\nFaerie Fire\nFog Cloud\nGoodberry\n"},
    {"Druid P2", "Healing Word\nJump\nLongstrider\nPurify Food and Drink\nSpeak with Animals\nThunderwave\n\0012nd Level\002\nAnimal Messenger\nBarkskin\nBeast Sense\nDarkvision\nEnhance Ability\nFind Traps\nFlame Blade\nFlaming Sphere\nGust of Wind\nHeat Metal\nHold Person\nLesser Restoration\nLocate Animals or Plants\n"},
    {"Druid P3", "Locate Object\nMoonbeam\nPass without Trace\nProtection from Poison\nSpike Growth\n\0013rd Level\002\nCall Lightning\nConjure Animals\nDaylight\nDispel Magic\nFeign Death\nMeld into Stone\nPlant Growth\nProtection from Energy\nSleet Storm\nSpeak with Plants\nWater Breathing\nWater Walk\nWind Wall\n\0014th Level\002\n"},
    {"Druid P4", "Blight\nConfusion\nConjure Minor Elementals\nConjure Woodland Beings\nControl Water\nDominate Beast\nFreedom of Movement\nGiant Insect\nHallucinatory Terrain\nIce Storm\nLocate Creature\nPolymorph\nStone Shape\nStoneskin\nWall of Fire\n\0015th Level\002\nAntilife Shell\nAwaken\nCommune with Nature\nConjure Elemental\n"},
    {"Druid P5", "Contagion\nGeas\nGreater Restoration\nInsect Plague\nMass Cure Wounds\nPlanar Binding\nReincarnate\nScrying\nTree Stride\nWall of Stone\n\0016th Level\002\nConjure Fey\nFind the Path\nHeal\nHeroes' Feast\nMove Earth\nSunbeam\nTransport via Plants\nWall of Thorns\nWind Walk\n"},
    {"Druid P6", "\0017th Level\002\nFire Storm\nMirage Arcane\nPlane Shift\nRegenerate\nReverse Gravity\n\0018th Level\002\nAnimal Shapes\nAntipathy/Sympathy\nControl Weather\nEarthquake\nFeeblemind\nSunburst\nTsunami\n\0019th Level\002\nForesight\nShapechange\nStorm of Vengeance\nTrue Resurrection\n"},
    {"Paladin P1", "\0011st Level\002\nBless\nCommand\nCompelled Duel\nCure Wounds\nDetect Evil and Good\nDetect Magic\nDetect Poison and Disease\nDivine Favor\nHeroism\nProtection from Evil and Good\nPurify Food and Drink\nSearing Smite\nShield of Faith\nThunderous Smite\nWrathful Smite\n\0012nd Level\002\nAid\nBranding Smite\nFind Steed\n"},
    {"Paladin P2", "Lesser Restoration\nLocate Object\nMagic Weapon\nProtection from Poison\nZone of Truth\n\0013rd Level\002\nAura of Vitality\nBlinding Smite\nCreate Food and Water\nCrusader's Mantle\nDaylight\nDispel Magic\nElemental Weapon\nMagic Circle\nRemove Curse\nRevivify\n\0014th Level\002\nAura of Life\nAura of Purity\nBanishment\n"},
    {"Paladin P3", "Death Ward\nLocate Creature\nStaggering Smite\n\0015th Level\002\nBanishing Smite\nCircle of Power\nDestructive Wave\nDispel Evil and Good\nGeas\nRaise Dead\n"},
    {"Ranger P1", "\0011st Level\002\nAlarm\nAnimal Friendship\nCure Wounds\nDetect Magic\nDetect Poison and Disease\nEnsnaring Strike\nFog Cloud\nGoodberry\nHail of Thorns\nHunter's Mark\nJump\nLongstrider\nSpeak with Animals\n\0012nd Level\002\nAnimal Messenger\nBarkskin\nBeast Sense\nCordon of Arrows\nDarkvision\n"},
    {"Ranger P2", "Find Traps\nLesser Restoration\nLocate Animals or Plants\nLocate Object\nPass without Trace\nProtection from Poison\nSilence\nSpike Growth\n\0013rd Level\002\nConjure Animals\nConjure Barrage\nDaylight\nLightning Arrow\nNondetection\nPlant Growth\nProtection from Energy\nSpeak with Plants\nWater Breathing\nWater Walk\nWind Wall\n"},
    {"Ranger P3", "\0014th Level\002\nConjure Woodland Beings\nFreedom of Movement\nGrasping Vine\nLocate Creature\nStoneskin\n\0015th Level\002\nCommune with Nature\nConjure Volley\nSwift Quiver\nTree Stride\n"},
    {"Sorcerer P1", "\001Cantrips (0 level)\002\nAcid Splash\nBlade Ward\nChill Touch\nDancing Lights\nFire Bolt\nLight\nMage Hand\nMending\nMessage\nMinor Illusion\nPoison Spray\nPrestidigitation\nRay of Frost\nShocking Grasp\nTrue Strike\n\0011st Level\002\nBurning Hands\nCharm Person\nChromatic Orb\n"},
    {"Sorcerer P2", "Color Spray\nComprehend Languages\nDetect Magic\nDisguise Self\nExpeditious Retreat\nFalse Life\nFeather Fall\nFog Cloud\nJump\nMage Armor\nMagic Missile\nShield\nSilent Image\nSleep\nThunderwave\nWitch Bolt\n\0012nd Level\002\nAlter Self\nBlindness/Deafness\nBlur\n"},
    {"Sorcerer P3", "Cloud of Daggers\nCrown of Madness\nDarkness\nDarkvision\nDetect Thoughts\nEnhance Ability\nEnlarge/Reduce\nGust of Wind\nHold Person\nInvisibility\nKnock\nLevitate\nMirror Image\nMisty Step\nPhantasmal Force\nScorching Ray\nSee Invisibility\nShatter\nSpider Climb\nSuggestion\n"},
    {"Sorcerer P4", "Web\n\0013rd Level\002\nBlink\nClairvoyance\nCounterspell\nDaylight\nDispel Magic\nFear\nFireball\nFly\nGaseous Form\nHaste\nHypnotic Pattern\nLightning Bolt\nMajor Image\nProtection from Energy\nSleet Storm\nSlow\nStinking Cloud\nTongues\n"},
    {"Sorcerer P5", "Water Breathing\nWater Walk\n\0014th Level\002\nBanishment\nBlight\nConfusion\nDimension Door\nDominate Beast\nGreater Invisibility\nIce Storm\nPolymorph\nStoneskin\nWall of Fire\n\0015th Level\002\nAnimate Objects\nCloudkill\nCone of Cold\nCreation\nDominate Person\nHold Monster\n"},
    {"Sorcerer P6", "Insect Plague\nSeeming\nTelekinesis\nTeleportation Circle\nWall of Stone\n\0016th Level\002\nArcane Gate\nChain Lightning\nCircle of Death\nDisintegrate\nEyebite\nGlobe of Invulnerability\nMass Suggestion\nMove Earth\nSunbeam\nTrue Seeing\n\0017th Level\002\nDelayed Blast Fireball\nEtherealness\nFinger of Death\n"},
    {"Sorcerer P7", "Fire Storm\nPlane Shift\nPrismatic Spray\nReverse Gravity\nTeleport\n\0018th Level\002\nDominate Monster\nEarthquake\nIncendiary Cloud\nPower Word Stun\nSunburst\n\0019th Level\002\nGate\nMeteor Swarm\nPower Word Kill\nTime Stop\nWish\n"},
    {"Warlock P1", "\001Cantrips (0 level)\002\nBlade Ward\nChill Touch\nEldritch Blast\nFriends\nMage Hand\nMinor Illusion\nPoison Spray\nPrestidigitation\nTrue Strike\n\0011st Level\002\nArmor of Agathys\nArms of Hadar\nCharm Person\nComprehend Languages\nExpeditious Retreat\nHellish Rebuke\nHex\nIllusory Script\nProtection from Evil and Good\n"},
    {"Warlock P2", "Unseen Servant\nWitch Bolt\n\0012nd Level\002\nCloud of Daggers\nCrown of Madness\nDarkness\nEnthrall\nHold Person\nInvisibility\nMirror Image\nMisty Step\nRay of Enfeeblement\nShatter\nSpider Climb\nSuggestion\n\0013rd Level\002\nCounterspell\nDispel Magic\nFear\nFly\n"},
    {"Warlock P3", "Gaseous Form\nHunder of Hadar\nHypnotic Pattern\nMagic Circle\nMajor Image\nRemove Curse\nTongues\nVampiric Touch\n\0014th Level\002\nBanishment\nBlight\nDimension Door\nHallucinatory Terrain\n\0015th Level\002\nContact Other Plane\nDream\nHold Monster\nScrying\n\0016th Level\002\nArcane Gate\n"},
    {"Warlock P4", "Circle of Death\nConjure Fey\nCreate Undead\nEyebite\nFlesh to Stone\nMass Suggestion\nTrue Seeing\n\0017th Level\002\nEtherealness\nFinger of Death\nForcecage\nPlane Shift\n\0018th Level\002\nDemiplane\nDominate Monster\nFeeblemind\nGlibness\nPower Word Stun\n\0019th Level\002\nAstral Projection\n"},
    {"Warlock P5", "Foresight\nImprisonment\nPower Word Kill\nTrue Polymorph\n"},
    {"Wizard P1", "\001Cantrips (0 level)\002\nAcid Splash\nBlade Ward\nChill Touch\nDancing Lights\nFire Bolt\nFriends\nLight\nMage Hand\nMending\nMessage\nMinor Illusion\nPoison Spray\nPrestidigitation\nRay of Frost\nShocking Grasp\nTrue Strike\n\0011st Level\002\nAlarm\nBurning Hands\n"},
    {"Wizard P2", "Charm Person\nChromatic Orb\nColor Spray\nComprehend Languages\nDetect Magic\nDisguise Self\nExpeditious Retreat\nFalse Life\nFeather Fall\nFind Familiar\nFog Cloud\nGrease\nIdentify\nIllusory Script\nJump\nLongstrider\nMage Armor\nMagic Missile\nProtection from Evil and Good\nRay of Sickness\n"},
    {"Wizard P3", "Shield\nSilent Image\nSleep\nTasha's Hideous Laughter\nTenser's Floating Disk\nThunderwave\nUnseen Servant\nWitch Bolt\n\0012nd Level\002\nAlter Self\nArcane Lock\nBlindness/Deafness\nBlur\nCloud of Daggers\nContinual Flame\nCrown of Madness\nDarkness\nDarkvision\nDetect Thoughts\nEnlarge/Reduce\n"},
    {"Wizard P4", "Flaming Sphere\nGentle Repose\nGust of Wind\nHold Person\nInvisibility\nKnock\nLevitate\nLocate Object\nMagic Mouth\nMagic Weapon\nMelf's Acid Arrow\nMirror Image\nMisty Step\nNystul's Magic Aura\nPhantasmal Force\nRay of Enfeeblement\nRope Trick\nScorching Ray\nSee Invisibility\nShatter\n"},
    {"Wizard P5", "Spider Climb\nSuggestion\nWeb\n\0013rd Level\002\nAnimate Dead\nBestow Curse\nBlink\nClairvoyance\nCounterspell\nDispel Magic\nFear\nFeign Death\nFireball\nFly\nGaseous Form\nGlyph of Warding\nHaste\nHypnotic Pattern\nLeomund's Tiny Hut\nLightning Bolt\n"},
    {"Wizard P6", "Magic Circle\nMajor Image\nNondetection\nPhantom Steed\nProtection from Energy\nRemove Curse\nSending\nSleet Storm\nSlow\nStinking Cloud\nTongues\nVampiric Touch\nWater Breathing\n\0014th Level\002\nArcane Eye\nBanishment\nBlight\nConfusion\nConjure Minor Elementals\nControl Water\n"},
    {"Wizard P7", "Dimension Door\nEvard's Black Tentacles\nFabricate\nFire Shield\nGreater Invisibility\nHallucinatory Terrain\nIce Storm\nLeomund's Secret Chest\nLocate Creature\nMordenkainen's Faithful Hound\nMordenkainen's Private Sanctum\nOtiluke's Resilient Sphere\nPhantasmal Killer\nPolymorph\nStone Shape\nStoneskin\nWall of Fire\n\0015th Level\002\nAnimate Objects\nBigby's Hand\n"},
    {"Wizard P8", "Cloudkill\nCone of Cold\nConjure Elemental\nContact Other Plane\nCreation\nDominate Person\nDream\nGeas\nHold Monster\nLegend Lore\nMislead\nModify Memory\nPasswall\nPlanar Binding\nRary's Telepathic Bond\nScrying\nSeeming\nTelekinesis\nTeleportation Circle\nWall of Force\n"},
    {"Wizard P9", "Wall of Stone\n\0016th Level\002\nArcane Gate\nChain Lightning\nCircle of Death\nContingency\nCreate Undead\nDisintegrate\nDrawmij's Instant Summons\nEyebite\nFlesh to Stone\nGlobe of Invulnerability\nGuards and Wards\nMagic Jar\nMass Suggestion\nMove Earth\nOtiluke's Freezing Sphere\nOtto's Irresistible Dance\nProgrammed Illusion\nSunbeam\n"},
    {"Wizard P10", "True Seeing\nWall of Ice\n\0017th Level\002\nDelayed Blast Fireball\nEtherealness\nFinger of Death\nForcecage\nMirage Arcane\nMordenkainen's Magnificent Mansion\nMordenkainen's Arcane Sword\nPlane Shift\nPrismatic Spray\nProject Image\nReverse Gravity\nSequester\nSimulacrum\nSymbol\nTeleport\n\0018th Level\002\nAntimagic Field\n"},
    {"Wizard P11", "Antipathy/Sympathy\nClone\nControl Weather\nDemiplane\nDominate Monster\nFeeblemind\nIncendiary Cloud\nMaze\nMind Blank\nPower Word Stun\nSunburst\nTelepathy\n\0019th Level\002\nAstral Projection\nForesight\nGate\nImprisonment\nMeteor Swarm\nPower Word Kill\nPrismatic Wall\n"},
    {"Wizard P12", "Shapechange\nTime Stop\nTrue Polymorph\nWeird\nWish\n"}
};
