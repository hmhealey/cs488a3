root = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- torso/hips/shoulders

torso = gr.sphere('torso')
torso:scale(2, 4, 2)
root:add_child(torso)

shoulder = gr.node('shoulder')
shoulder:translate(0, 2, 0)
root:add_child(shoulder)

shoulderShape = gr.sphere('shoulderShape')
shoulderShape:scale(4, 1, 1)
shoulder:add_child(shoulderShape)

hips = gr.node('hips')
hips:translate(0, -3, 0)
root:add_child(hips)

hipShape = gr.sphere('hipShape')
hipShape:scale(3, 1, 1)
hips:add_child(hipShape)

-- neck/head/nose

neck = gr.node('neck')
neck:translate(0, 2, 0)
shoulder:add_child(neck)

neckShape = gr.sphere('neckShape')
neckShape:scale(1, 2, 1)
neck:add_child(neckShape)

head = gr.node('head')
head:translate(0, 1.5, 0)
neck:add_child(head)

headShape = gr.sphere('headShape')
headShape:scale(2, 2, 2)
head:add_child(headShape)

noseShape = gr.sphere('noseShape')
noseShape:scale(0.4, 0.4, 0.4)
noseShape:translate(0, 0, 2)
head:add_child(noseShape)

-- arms

leftArm = gr.node('leftArm')
leftArm:translate(0, -3, 0)
shoulder:add_child(leftArm)

leftArmShape = gr.sphere('leftArmShape')
leftArmShape:scale(1, 3, 1)
leftArm:add_child(leftArmShape)

leftForearm = gr.node('leftForearm')
leftForearm:translate(0, -3, 0)
leftArm:add_child(leftForearm)

leftForearmShape = gr.sphere('leftForearmShape')
leftForearmShape:scale(1, 3, 1)
leftForearm:add_child(leftForearmShape)

leftHandShape = gr.sphere(



return root
