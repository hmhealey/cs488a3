root = gr.node('root')

skin = gr.material({200.0 / 255.0, 172.0 / 255.0, 163.0 / 255.0}, {0.1, 0.1, 0.1}, 10)
nose = gr.material({0.9 * 200.0 / 255.0, 0.8 * 172.0 / 255.0, 0.8 * 163.0 / 255.0}, {0.1, 0.1, 0.1}, 10)
shirt = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 50)
pants = gr.material({0.1, 0.1, 0.4}, {0.1, 0.1, 0.1}, 50)
shoes = gr.material({0.2, 0.2, 0.2}, {0.1, 0.1, 0.1}, 10)

-- torso/hips/shoulders

torso = gr.node('torso')
torso:translate(0, 2, 0)
root:add_child(torso)

torsoShape = gr.sphere('torsoShape')
torsoShape:scale(2, 4, 2)
torsoShape:set_material(shirt)
torso:add_child(torsoShape)

shoulder = gr.node('shoulder')
shoulder:translate(0, 2, 0)
torso:add_child(shoulder)

shoulderShape = gr.sphere('shoulderShape')
shoulderShape:scale(4, 1, 1)
shoulderShape:set_material(shirt)
shoulder:add_child(shoulderShape)

hips = gr.node('hips')
hips:translate(0, -3, 0)
torso:add_child(hips)

hipShape = gr.sphere('hipShape')
hipShape:scale(2.5, 1, 1)
hipShape:set_material(pants)
hips:add_child(hipShape)

-- neck/head/nose

sternum = gr.joint('sternum', {-15, 0, 15}, {0, 0, 0}) -- that's right! the sternum is now a joint!
shoulder:add_child(sternum)

neck = gr.node('neck')
neck:translate(0, 2, 0)
sternum:add_child(neck)

neckShape = gr.sphere('neckShape')
neckShape:translate(0, 2, 0)
neckShape:scale(1, 2, 1)
neckShape:set_material(skin)
sternum:add_child(neckShape)

head = gr.joint('head', {-15, 0, 15}, {-45, 0, 45})
head:translate(0, 1.5, 0)
neck:add_child(head)

headShape = gr.sphere('headShape')
headShape:scale(2, 2, 2)
headShape:set_material(skin)
head:add_child(headShape)

face = gr.node('face') -- prevents clicking on the nose to grab the whole head
head:add_child(face)

noseShape = gr.sphere('noseShape')
noseShape:scale(0.4, 0.4, 0.4)
noseShape:translate(0, 0, 5)
noseShape:set_material(nose)
face:add_child(noseShape)

-- left arm

leftShoulder = gr.joint('leftShoulder', {-120, 0, 30}, {0, 0, 0})
leftShoulder:translate(-3.5, 0, 0)
shoulder:add_child(leftShoulder)

leftArm = gr.node('leftArm')
leftArm:translate(0, -2, 0)
leftShoulder:add_child(leftArm)

leftArmShape = gr.sphere('leftArmShape')
leftArmShape:translate(0, -2, 0)
leftArmShape:scale(1, 2, 1)
leftArmShape:set_material(shirt)
leftShoulder:add_child(leftArmShape)

leftElbow = gr.joint('leftElbow', {-135, 0, 0}, {0, 0, 0})
leftElbow:translate(0, -1, 0)
leftArm:add_child(leftElbow)

leftForearm = gr.node('leftForearm')
leftForearm:translate(0, -1, 0)
leftElbow:add_child(leftForearm)

leftForearmShape = gr.sphere('leftForearmShape')
leftForearmShape:translate(0, -1, 0)
leftForearmShape:scale(1, 2, 1)
leftForearmShape:set_material(skin)
leftElbow:add_child(leftForearmShape)

leftWrist = gr.joint('leftWrist', {-60, 0, 60}, {0, 0, 0})
leftWrist:translate(0, -1, 0)
leftForearm:add_child(leftWrist)

leftHand = gr.node('leftHand')
leftHand:translate(0, -1, 0)
leftWrist:add_child(leftHand)

leftHandShape = gr.sphere('leftHandShape')
leftHandShape:translate(0, -1, 0)
leftHandShape:scale(0.75, 0.75, 0.75)
leftHandShape:set_material(skin)
leftWrist:add_child(leftHandShape)

-- right arm

rightShoulder = gr.joint('rightShoulder', {-120, 0, 30}, {0, 0, 0})
rightShoulder:translate(3.5, 0, 0)
shoulder:add_child(rightShoulder)

rightArm = gr.node('rightArm')
rightArm:translate(0, -2, 0)
rightShoulder:add_child(rightArm)

rightArmShape = gr.sphere('rightArmShape')
rightArmShape:translate(0, -2, 0)
rightArmShape:scale(1, 2, 1)
rightArmShape:set_material(shirt)
rightShoulder:add_child(rightArmShape)

rightElbow = gr.joint('rightElbow', {-135, 0, 0}, {0, 0, 0})
rightElbow:translate(0, -1, 0)
rightArm:add_child(rightElbow)

rightForearm = gr.node('rightForearm')
rightForearm:translate(0, -1, 0)
rightElbow:add_child(rightForearm)

rightForearmShape = gr.sphere('rightForearmShape')
rightForearmShape:translate(0, -1, 0)
rightForearmShape:scale(1, 2, 1)
rightForearmShape:set_material(skin)
rightElbow:add_child(rightForearmShape)

rightWrist = gr.joint('rightWrist', {-60, 0, 60}, {0, 0, 0})
rightWrist:translate(0, -1, 0)
rightForearm:add_child(rightWrist)

rightHand = gr.node('rightHand')
rightHand:translate(0, -1, 0)
rightWrist:add_child(rightHand)

rightHandShape = gr.sphere('rightHandShape')
rightHandShape:translate(0, -1, 0)
rightHandShape:scale(0.75, 0.75, 0.75)
rightHandShape:set_material(skin)
rightWrist:add_child(rightHandShape)

-- left leg

leftHip = gr.joint('leftHip', {-45, 0, 45}, {0, 0, 0})
leftHip:translate(-2, 0, 0)
hips:add_child(leftHip)

leftThigh = gr.node('leftThigh')
leftThigh:translate(0, -3, 0)
leftHip:add_child(leftThigh)

leftThighShape = gr.sphere('leftThighShape')
leftThighShape:translate(0, -3, 0)
leftThighShape:scale(1, 3, 1)
leftThighShape:set_material(pants)
leftHip:add_child(leftThighShape)

leftKnee = gr.joint('leftKnee', {0, 0, 90}, {0, 0, 0})
leftKnee:translate(0, -2.5, 0)
leftThigh:add_child(leftKnee)

leftCalf = gr.node('leftCalf', {0, 0, 60}, {0, 0, 0})
leftCalf:translate(0, -1.5, 0)
leftKnee:add_child(leftCalf)

leftCalfShape = gr.sphere('leftCalfShape')
leftCalfShape:translate(0, -1.5, 0)
leftCalfShape:scale(1, 2, 1)
leftCalfShape:set_material(pants)
leftKnee:add_child(leftCalfShape)

leftFoot = gr.joint('leftFoot', {-30, 0, 30}, {0, 0, 0})
leftFoot:translate(0, -2.5, 0)
leftCalf:add_child(leftFoot)

leftFootShape = gr.sphere('leftFootShape')
leftFootShape:translate(0, 0, 0.75)
leftFootShape:scale(1, 1, 1.5)
leftFootShape:set_material(shoes)
leftFoot:add_child(leftFootShape)

-- right leg

rightHip = gr.joint('rightHip', {-45, 0, 45}, {0, 0, 0})
rightHip:translate(2, 0, 0)
hips:add_child(rightHip)

rightThigh = gr.node('rightThigh')
rightThigh:translate(0, -3, 0)
rightHip:add_child(rightThigh)

rightThighShape = gr.sphere('rightThighShape')
rightThighShape:translate(0, -3, 0)
rightThighShape:scale(1, 3, 1)
rightThighShape:set_material(pants)
rightHip:add_child(rightThighShape)

rightKnee = gr.joint('rightKnee', {0, 0, 90}, {0, 0, 0})
rightKnee:translate(0, -2.5, 0)
rightThigh:add_child(rightKnee)

rightCalf = gr.node('rightCalf', {0, 0, 60}, {0, 0, 0})
rightCalf:translate(0, -1.5, 0)
rightKnee:add_child(rightCalf)

rightCalfShape = gr.sphere('rightCalfShape')
rightCalfShape:translate(0, -1.5, 0)
rightCalfShape:scale(1, 2, 1)
rightCalfShape:set_material(pants)
rightKnee:add_child(rightCalfShape)

rightFoot = gr.joint('rightFoot', {-30, 0, 30}, {0, 0, 0})
rightFoot:translate(0, -2.5, 0)
rightCalf:add_child(rightFoot)

rightFootShape = gr.sphere('rightFootShape')
rightFootShape:translate(0, 0, 0.75)
rightFootShape:scale(1, 1, 1.5)
rightFootShape:set_material(shoes)
rightFoot:add_child(rightFootShape)

return root
