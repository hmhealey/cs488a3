root = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- torso/hips/shoulders

torso = gr.node('torso')
torso:translate(0, 2, 0)
root:add_child(torso)

torsoChild = gr.sphere('torsoChild')
torsoChild:scale(2, 4, 2)
torso:add_child(torsoChild)

shoulder = gr.node('shoulder')
shoulder:translate(0, 2, 0)
torso:add_child(shoulder)

shoulderShape = gr.sphere('shoulderShape')
shoulderShape:scale(4, 1, 1)
shoulder:add_child(shoulderShape)

hips = gr.node('hips')
hips:translate(0, -3, 0)
torso:add_child(hips)

hipShape = gr.sphere('hipShape')
hipShape:scale(2.5, 1, 1)
hips:add_child(hipShape)

-- neck/head/nose

neck = gr.joint('neck', {-15, 0, 40}, {0, 0, 0})
neck:translate(0, 2, 0)
shoulder:add_child(neck)

neckShape = gr.sphere('neckShape')
neckShape:scale(1, 2, 1)
neck:add_child(neckShape)

head = gr.joint('head', {-15, 0, 15}, {-45, 0, 45})
head:translate(0, 1.5, 0)
neck:add_child(head)

headShape = gr.sphere('headShape')
headShape:scale(2, 2, 2)
head:add_child(headShape)

noseShape = gr.sphere('noseShape')
noseShape:scale(0.4, 0.4, 0.4)
noseShape:translate(0, 0, 5)
head:add_child(noseShape)

-- left arm

leftArm = gr.joint('leftArm', {-15, 0, 120}, {0, 0, 0})
leftArm:translate(-3.5, -2, 0)
shoulder:add_child(leftArm)

leftArmShape = gr.sphere('leftArmShape')
leftArmShape:scale(1, 2.5, 1)
leftArm:add_child(leftArmShape)

leftForearm = gr.joint('leftForearm', {0, 0, 135}, {0, 0, 0})
leftForearm:rotate('y', -90)
leftForearm:translate(0, -2, 0)
leftArm:add_child(leftForearm)

leftForearmShape = gr.sphere('leftForearmShape')
leftForearmShape:scale(1, 2, 1)
leftForearm:add_child(leftForearmShape)

leftHand = gr.joint('leftHand', {-60, 0, 60}, {0, 0, 90})
leftHand:translate(0, -2, 0)
leftForearm:add_child(leftHand)

leftHandShape = gr.sphere('leftHandShape')
leftHandShape:scale(0.75, 0.75, 0.75)
leftHand:add_child(leftHandShape)

-- right arm

rightArm = gr.joint('rightArm', {-15, 0, 120}, {0, 0, 0})
rightArm:translate(3.5, -2, 0)
shoulder:add_child(rightArm)

rightArmShape = gr.sphere('rightArmShape')
rightArmShape:scale(1, 2.5, 1)
rightArm:add_child(rightArmShape)

rightForearm = gr.joint('rightForearm', {0, 0, 135}, {0, 0, 0})
rightForearm:rotate('y', 90)
rightForearm:translate(0, -2, 0)
rightArm:add_child(rightForearm)

rightForearmShape = gr.sphere('rightForearmShape')
rightForearmShape:scale(1, 2, 1)
rightForearm:add_child(rightForearmShape)

rightHand = gr.joint('rightHand', {-60, 0, 60}, {0, 0, 90})
rightHand:translate(0, -2, 0)
rightForearm:add_child(rightHand)

rightHandShape = gr.sphere('rightHandShape')
rightHandShape:scale(0.75, 0.75, 0.75)
rightHand:add_child(rightHandShape)

-- left leg

leftThigh = gr.joint('leftThigh', {-45, 0, 45}, {0, 0, 0})
leftThigh:translate(-2, -3, 0)
hips:add_child(leftThigh)

leftThighShape = gr.sphere('leftThighShape')
leftThighShape:scale(1, 3, 1)
leftThigh:add_child(leftThighShape)

leftCalf = gr.joint('leftCalf', {0, 0, 60}, {0, 0, 0})
leftCalf:translate(0, -3, 0)
leftThigh:add_child(leftCalf)

leftCalfShape = gr.sphere('leftCalfShape')
leftCalfShape:scale(1, 3, 1)
leftCalf:add_child(leftCalfShape)

leftFoot = gr.joint('leftFoot', {-15, 0, 15}, {0, 0, 0})
leftFoot:translate(-1, -2.5, 1)
leftFoot:rotate('y', 45)
leftCalf:add_child(leftFoot)

leftFootShape = gr.sphere('leftFootShape')
leftFootShape:scale(2, 1, 1)
leftFoot:add_child(leftFootShape)

-- right leg

rightThigh = gr.joint('rightThigh', {-45, 0, 45}, {0, 0, 0})
rightThigh:translate(2, -3, 0)
hips:add_child(rightThigh)

rightThighShape = gr.sphere('rightThighShape')
rightThighShape:scale(1, 3, 1)
rightThigh:add_child(rightThighShape)

rightCalf = gr.joint('rightCalf', {0, 0, 60}, {0, 0, 0})
rightCalf:translate(0, -3, 0)
rightThigh:add_child(rightCalf)

rightCalfShape = gr.sphere('rightCalfShape')
rightCalfShape:scale(1, 3, 1)
rightCalf:add_child(rightCalfShape)

rightFoot = gr.joint('rightFoot', {-15, 0, 15}, {0, 0, 0})
rightFoot:translate(1, -2.5, 1)
rightFoot:rotate('y', -45)
rightCalf:add_child(rightFoot)

rightFootShape = gr.sphere('rightFootShape')
rightFootShape:scale(2, 1, 1)
rightFoot:add_child(rightFootShape)

return root
