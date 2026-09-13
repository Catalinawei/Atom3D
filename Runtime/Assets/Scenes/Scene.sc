# =========================================
# Physics obstacle course - REVERSED
# NO RAMPS - CharacterController safe
# Course goes toward +Z
# =========================================


# =========================================
# PLAYER
# =========================================

newobj Person
p 0.0 3.0 0.0
r 0.0 0.0 0.0
s 0.5 0.5 0.5
mesh Assets/Objects/model.obj
p_mass 50
new_comp charController


# =========================================
# STARTING AREA
# =========================================

newobj StartFloor
p 0.0 -1.0 0.0
r 0.0 0.0 0.0
s 5.0 0.5 5.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# STAIRS
# =========================================

newobj Step1
p 0.0 -0.5 7.0
r 0.0 0.0 0.0
s 3.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Step2
p 0.0 0.0 10.0
r 0.0 0.0 0.0
s 3.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Step3
p 0.0 0.5 13.0
r 0.0 0.0 0.0
s 3.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Step4
p 0.0 1.0 16.0
r 0.0 0.0 0.0
s 3.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Step5
p 0.0 1.5 19.0
r 0.0 0.0 0.0
s 3.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# HIGH PLATFORM
# =========================================

newobj HighPlatform
p 0.0 1.5 24.0
r 0.0 0.0 0.0
s 5.0 0.5 4.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# NARROW BRIDGE
# =========================================

newobj Bridge1
p 0.0 1.5 30.0
r 0.0 0.0 0.0
s 1.2 0.35 3.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Bridge2
p 1.5 1.5 35.0
r 0.0 0.0 20.0
s 1.0 0.35 3.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Bridge3
p -1.0 1.5 40.0
r 0.0 0.0 -20.0
s 1.0 0.35 3.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# CHECKPOINT
# =========================================

newobj Checkpoint
p 0.0 1.5 45.0
r 0.0 0.0 0.0
s 4.0 0.5 3.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# JUMP GAPS
# =========================================

newobj JumpPlatform1
p -2.0 1.5 51.0
r 0.0 0.0 0.0
s 2.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj JumpPlatform2
p 2.0 1.5 56.0
r 0.0 0.0 0.0
s 2.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj JumpPlatform3
p -2.0 1.5 61.0
r 0.0 0.0 0.0
s 2.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj JumpPlatform4
p 2.0 1.5 66.0
r 0.0 0.0 0.0
s 2.0 0.5 2.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# PILLAR JUMPS
# =========================================

newobj Pillar1
p -3.0 0.0 72.0
r 0.0 0.0 0.0
s 1.5 2.0 1.5
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Pillar2
p 1.5 1.0 76.0
r 0.0 0.0 0.0
s 1.5 3.0 1.5
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Pillar3
p -2.0 2.0 80.0
r 0.0 0.0 0.0
s 1.5 4.0 1.5
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Pillar4
p 2.5 1.0 84.0
r 0.0 0.0 0.0
s 1.5 3.0 1.5
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# ZIG-ZAG PLATFORMS
# =========================================

newobj Zig1
p -4.0 3.5 89.0
r 0.0 25.0 0.0
s 3.0 0.4 1.3
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Zig2
p 2.0 3.5 93.0
r 0.0 -25.0 0.0
s 3.0 0.4 1.3
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Zig3
p -3.0 3.5 97.0
r 0.0 25.0 0.0
s 3.0 0.4 1.3
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Zig4
p 3.0 3.5 101.0
r 0.0 -25.0 0.0
s 3.0 0.4 1.3
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# BALANCE BEAM
# =========================================

newobj BalanceBeam
p 0.0 3.5 108.0
r 0.0 0.0 0.0
s 0.6 0.4 6.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# FINAL JUMP SECTION
# =========================================

newobj FinalJump1
p -2.0 3.5 117.0
r 0.0 0.0 0.0
s 2.0 0.4 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj FinalJump2
p 2.0 3.5 122.0
r 0.0 0.0 0.0
s 2.0 0.4 2.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj FinalJump3
p 0.0 3.5 127.0
r 0.0 0.0 0.0
s 2.5 0.4 2.5
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# FINISH PLATFORM
# =========================================

newobj Finish
p 0.0 4.0 133.0
r 0.0 0.0 0.0
s 6.0 0.5 5.0
mesh Assets/Objects/test.obj
p_mass 0.0


# =========================================
# FINISH WALLS
# =========================================

newobj FinishWallLeft
p -6.0 6.0 133.0
r 0.0 0.0 0.0
s 0.5 2.0 5.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj FinishWallRight
p 6.0 6.0 133.0
r 0.0 0.0 0.0
s 0.5 2.0 5.0
mesh Assets/Objects/test.obj
p_mass 0.0

newobj FinishWallBack
p 0.0 6.0 138.0
r 0.0 0.0 0.0
s 6.0 2.0 0.5
mesh Assets/Objects/test.obj
p_mass 0.0