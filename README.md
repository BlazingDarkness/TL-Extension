# TL-Extension

This is a wrapper around UCLAN's TL-Engine which is a minimal 3D graphics engine used within their course modules and assignments.

Since the TL-Engine is mostly closed sourced the option to extend it with useful features is limited.
This wrapper adds the following:

# Sound and Music
SFML's music and sounds are added to the engine in a similar fashion that Meshes and Models exist.
These can be given individual volumes as well as global and categorical volume multipliers

# Particle System
Particle emitters are configurable to emit particles with a pattern, duration, velocity, and sprite/animation.

Relatistically a good particle system would be done on the GPU, but with the heavy limitation of not having access to that subsystem of the TL-Engine the only solution to bolt on a particle system is creating quad modules which is noticably costly in performance when in large enough numbers.

# Model and Skin caches
A limitation of the TL-Engine is the stalling whenever a model's skin is changed. The solution used here is to create a cache of models positioned behind the camra (so not rendered) that already have the skins;. This way instead of swapping skins we can swap the two models.

# Animation
Sprite based animations.
