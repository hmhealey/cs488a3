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

leftShoulder = gr.joint('leftShoulder', {-15, 0, 120}, {0, 0, 0})
leftShoulder:translate(-3.5, 0, 0)
shoulder:add_child(leftShoulder)

leftArm = gr.node('leftArm')
leftArm:translate(0, -2, 0)
leftShoulder:add_child(leftArm)

leftArmShape = gr.sphere('leftArmShape')
leftArmShape:translate(0, -2, 0)
leftArmShape:scale(1, 2.5, 1)
leftShoulder:add_child(leftArmShape)

leftElbow = gr.joint('leftElbow', {0, 0, 135}, {0, 0, 0})
leftElbow:translate(0, -1, 0)
leftArm:add_child(leftElbow)

leftForearm = gr.node('leftForearm')
--leftForearm:rotate('y', -90)
leftForearm:translate(0, -1, 0)
leftElbow:add_child(leftForearm)

leftForearmShape = gr.sphere('leftForearmShape')
leftForearmShape:translate(0, -1, 0)
leftForearmShape:scale(1, 2, 1)
leftElbow:add_child(leftForearmShape)

leftWrist = gr.joint('leftWrist', {-60, 0, 60}, {0, 0, 90})
leftWrist:translate(0, -1, 0)
leftForearm:add_child(leftWrist)

leftHand = gr.node('leftHand')
leftHand:translate(0, -1, 0)
leftWrist:add_child(leftHand)

leftHandShape = gr.sphere('leftHandShape')
leftHandShape:translate(0, -1, 0)
leftHandShape:scale(0.75, 0.75, 0.75)
leftWrist:add_child(leftHandShape)

-- right arm

rightShoulder = gr.joint('rightShoulder', {-15, 0, 120}, {0, 0, 0})
rightShoulder:translate(3.5, 0, 0)
shoulder:add_child(rightShoulder)

rightArm = gr.node('rightArm')
rightArm:translate(0, -2, 0)
rightShoulder:add_child(rightArm)

rightArmShape = gr.sphere('rightArmShape')
rightArmShape:translate(0, -2, 0)
rightArmShape:scale(1, 2.5, 1)
rightShoulder:add_child(rightArmShape)

rightElbow = gr.joint('rightElbow', {0, 0, 135}, {0, 0, 0})
rightElbow:translate(0, -1, 0)
rightArm:add_child(rightElbow)

rightForearm = gr.node('rightForearm')
--rightForearm:rotate('y', -90)
rightForearm:translate(0, -1, 0)
rightElbow:add_child(rightForearm)

rightForearmShape = gr.sphere('rightForearmShape')
rightForearmShape:translate(0, -1, 0)
rightForearmShape:scale(1, 2, 1)
rightElbow:add_child(rightForearmShape)

rightWrist = gr.joint('rightWrist', {-60, 0, 60}, {0, 0, 90})
rightWrist:translate(0, -1, 0)
rightForearm:add_child(rightWrist)

rightHand = gr.node('rightHand')
rightHand:translate(0, -1, 0)
rightWrist:add_child(rightHand)

rightHandShape = gr.sphere('rightHandShape')
rightHandShape:translate(0, -1, 0)
rightHandShape:scale(0.75, 0.75, 0.75)
rightWrist:add_child(rightHandShape)

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
