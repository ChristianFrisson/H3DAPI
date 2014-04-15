//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file ParticleSystem.h
/// \brief Header file for ParticleSystem, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <H3D/X3DShapeNode.h>
#include <H3D/X3DParticleEmitterNode.h>
#include <H3D/X3DParticlePhysicsModelNode.h>
#include <H3D/TextureCoordinate.h>
#include <H3D/X3DColorNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>
#include <H3D/MFFloat.h>
#include <H3D/SFVec2f.h>

// stl includes
#include <list>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class ParticleSystem
  /// \brief The ParticleSystem node specifies a complete particle system.
  ///
  /// The geometryType field specifies the type of geometry that should be 
  /// used to represent individual particles. Typically, a particle is 
  /// calculated as a point in space at which the geometry is placed and then
  /// rendered using the appearance attributes.
  ///
  /// The types of geometry are defined to render in the following way:
  /// - "LINE":  A line is drawn along the particle's current velocity vector,
  /// for this frame, centered about the particle's position. The length of the
  /// line is specified by the particle's height from the particleSize field 
  /// value.
  /// - "POINT":  A point geometry is rendered at the particle's position.
  /// - "QUAD":  A 2D quad is rendered aligned in the local coordinate space 
  /// of the particle system with the face normal pointing along the positive
  /// Z axis. Individual quads are not aligned to the user's eye position but
  /// are effected in depth by the physics model. The particle's position is at
  /// the center of the quad.
  /// - "SPRITE":  A point sprite that uses a 2D point position to locate a
  /// screen-aligned quad at the center of the particle's location is rendered.
  /// - "TRIANGLE":  A 2D quad is rendered using a pair of triangles aligned in
  /// the local coordinate space of the particle system with the face normal
  /// pointing along the positive Z axis. Individual triangles are not aligned
  /// to the user's eye position, but are effected in depth by the physics model.
  /// The particle's position is at the center of the triangle.
  /// - "GEOMETRY":  The geometry specified by the geometry field is rendered 
  /// for each particle using the local coordinate system. Changing the value
  /// of the geometry field or the definition of the geometry node shall take
  /// effect this frame.
  ///
  /// The geometry field specifies the geometry to be used for each particle
  /// when the geometryType field has value "GEOMETRY".
  ///
  /// The appearance field holds information that is used for the geometry. All
  /// effects, such as material colours and/or multi-textures, are applied to 
  /// each particle. If a texture coordinate ramp and key is supplied with this
  /// geometry, it shall be used in preference to any automatic texture 
  /// coordinate generation. If automatic texture coordinate generation is 
  /// used, results shall be based on the entire volume that the particles 
  /// consume, not locally applied to each particle.
  ///
  /// Procedural shaders may also be supplied. The particle system shall manage
  /// the position of all particles each frame. This position becomes the 
  /// initial geometry input to the shader.
  ///
  /// The emitter field specifies the type of emitter geometry and properties 
  /// that the particles are given for their initial positions. After being 
  /// created, the individual particles are then manipulated according to the
  /// physics model(s) specified in the physics field.
  ///
  /// The colorRamp and colorKey fields specify how to change the base colour
  /// of the particle over the lifetime of an individual particle. The colorKey
  /// field represents the time of the particle in seconds, while the colorRamp
  /// field holds a series of colour values to be used at the given key points
  /// in time. Between keys, colour values are interpreted in a linear HSV 
  /// space, using the same rules defined for the ColorInterpolator node. The
  /// colour values are defined as per-vertex colour values. Consequently, if 
  /// an appearance node with material is provided, the material properties 
  /// will override the colour ramp.
  ///
  /// The isActive outputOnly field indicates whether the particle system is
  /// currently running, based on the setup of the node.
  ///
  /// The isActive field sends a value of FALSE when activity has stopped 
  /// occurring. A particle system without an emitter set can never be active.
  /// If the emitter is defined by an EXTERNPROTO that has not yet resolved, 
  /// isActive shall initially be FALSE, until the point the EXTERNPROTO has 
  /// loaded and is verified as being a correct node type. If these validity 
  /// checks pass, isActive is set to TRUE and this defines the local time zero
  /// to start the particle effects.
  ///
  /// The enabled field controls whether this ParticleSystem is currently 
  /// active and rendering particles this frame. Setting this value to FALSE
  /// will immediately remove all visible particles from the scene from the
  /// next frame onwards. Setting the field to TRUE will start the system again
  /// from a local time zero. It does not start off from where it was 
  /// previously. In doing so, it will issue another value of TRUE for 
  /// isActive. If a value of FALSE is set for enabled, isActive will also be
  /// set to FALSE. 
  ///
  /// The createParticles field is used to control whether any further new 
  /// particles should be created. This allows the user to stop production
  /// of new particles, but keep those already existing in the scene to 
  /// continue to animate. This differs from the enabled field that would
  /// immediately remove all particles. The createParticles field keeps the
  /// existing particles in existence until the end of their lifetimes. If
  /// there are no particles left in the scene, the system is still considered
  /// both active and enabled.
  ///
  /// The particleSize field describes the dimensions in metres of the width
  /// and height of each particle. Changing this value dynamically will only
  /// change new particles created after the change. Particles created before
  /// this timestamp will remain at the old size. This field only effects
  /// particles using "LINE", "QUAD", "SPRITE", and "TRIANGLE" geometry types.
  ///
  /// The texCoordRamp and texCoordKey fields control the texture coordinates
  /// of the provided texture(s) in the Appearance node, over time. Particle
  /// systems frequently like to change the texture on a particle as it ages,
  /// yet there is no good way of accomplishing this through standard
  /// interpolators because interpolators have no concept of particle time. 
  /// This pair of fields hold time-dependent values for the texture 
  /// coordinates to be applied to the particle. When a particle reaches the
  /// next time stamp it moves to the next set of texture coordinates. There
  /// is no interpolation of the texture coordinates, just sequenced according
  /// to the times defined by texCoordKey.
  ///
  /// The node placed in texCoordRamp shall have enough values to work with 
  /// the numbers required by geometryType. The following numbers and rules
  /// for mapping texture coordinates to the quad shall be used:
  /// - "LINE": The coordinates are paired such that the coordinate with
  /// lowest value index is associated with the end of the line that is
  /// closest to the emitter location and the coordinate with the next 
  /// higher index is associated with the end of the line furthest from
  /// the emitter location. Each timestamp increases the index into the ramp
  /// by two.
  /// - "POINT": Texture coordinates are ignored.
  /// - "QUAD":  Assuming a quad facing the current viewer position, 
  /// coordinates are defined in a counter-clockwise order starting at the
  /// lower-left corner.
  /// - "SPRITE": Texture coordinates are ignored for this type. Each 
  /// particle uses the entire supplied texture.
  /// - "TRIANGLE": Assuming two triangles facing the user, only four 
  /// coordinates are supplied, representing the four corners of the
  /// quad. The order is the same as for "QUAD".
  /// - "GEOMETRY": Texture coordinates ramps are ignored for this type.
  /// Texture coordinates from the geometry representation are used or 
  /// automatic texture coordinate generation from the appearance node
  /// is used.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ParticleSystem.x3d">ParticleSystem.x3d</a>
  ///     ( <a href="examples/ParticleSystem.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ParticleSystem.dot
  class H3DAPI_API ParticleSystem : 
    public X3DShapeNode {
  public:

    typedef TypedSFNode< X3DParticleEmitterNode > SFEmitterNode;
    typedef TypedSFNode< X3DColorNode > SFColorNode;
    typedef TypedSFNode< TextureCoordinate > SFTextureCoordinate;
    typedef TypedMFNode< X3DParticlePhysicsModelNode > MFPhysicsModelNode;
    
    /// Constructor.
    ParticleSystem( Inst< SFAppearanceNode > _appearance        = 0,
                    Inst< SFGeometryNode   > _geometry          = 0,
                    Inst< SFHapticGeometry > _hapticGeometry    = 0,
                    Inst< SFNode           > _metadata          = 0,
                    Inst< SFBound          > _bound             = 0,
                    Inst< SFVec3f          > _bboxCenter        = 0,
                    Inst< SFVec3f          > _bboxSize          = 0,
                    Inst< DisplayList      > _displayList       = 0,
                    Inst< SFBool           > _createParticles   = 0,
                    Inst< SFBool           > _enabled           = 0,
                    Inst< SFFloat          > _lifetimeVariation = 0,
                    Inst< SFInt32          > _maxParticles      = 0,
                    Inst< SFFloat          > _particleLifetime  = 0,
                    Inst< SFVec2f          > _particleSize      = 0,
                    Inst< SFBool           > _isActive          = 0,
                    Inst< SFColorNode      > _colorRamp         = 0,
                    Inst< MFFloat          > _colorKey          = 0,
                    Inst< SFEmitterNode    > _emitter           = 0,
                    Inst< SFString         > _geometryType      = 0,
                    Inst< MFPhysicsModelNode > _physics         = 0,
                    Inst< SFTextureCoordinate> _texCoordRamp    = 0,
                    Inst< MFFloat          > _texCoordKey       = 0
                    );
    virtual void initialize();
    
    /// Render the shape using OpenGL.
    virtual void render();

    /// Traverse the scenegraph. Calls traverseSG on appeance and geometry.
    virtual void traverseSG( TraverseInfo &ti );

    /// Returns the enum equivalent to the particle type specified with the
    /// geometryType field.
    X3DParticleEmitterNode::Particle::ParticleType getCurrentParticleType();

    /// Returns a random value in the range main_value +- (variation * main_value )  
    static H3DFloat getVariationValue( H3DFloat main_value, H3DFloat variation ); 

    /// Returns a random value in the given range.
    static H3DFloat getRandomValue( H3DFloat min, H3DFloat max );

    /// Returns a random point on the unit sphere.
    static Vec3f getRandomPointOnUnitSphere();

    /// The createParticles field is used to control whether any further new 
    /// particles should be created. This allows the user to stop production
    /// of new particles, but keep those already existing in the scene to 
    /// continue to animate. This differs from the enabled field that would
    /// immediately remove all particles. 
    /// 
    /// <b>Access type:</b> inputOutput
    ///
    /// <b>Default value:</b> TRUE
    /// 
    /// \dotfile ParticleSystem_createParticles.dot
    auto_ptr< SFBool > createParticles;

    /// The enabled field controls whether this ParticleSystem is currently 
    /// active and rendering particles this frame. Setting this value to FALSE
    /// will immediately remove all visible particles from the scene from the
    /// next frame onwards. Setting the field to TRUE will start the system 
    /// again from a local time zero. It does not start off from where it was 
    /// previously. In doing so, it will issue another value of TRUE for 
    /// isActive. If a value of FALSE is set for enabled, isActive will also be
    /// set to FALSE. 
    /// 
    /// <b>Access type:</b> inputOutput
    ///
    /// <b>Default value:</b> TRUE
    /// 
    /// \dotfile ParticleSystem_enabled.dot
    auto_ptr< SFBool > enabled;

    /// The lifetimeVariation field controls the variation in a particles
    /// lifetime. It describes the allowed deviation in particle liftime 
    /// from the value in particleLifetime. The variation is the maximum
    /// bound of that value, described as a proportion of the original 
    /// value. A variation value of zero does not allow any randomness.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0.25
    /// 
    /// \dotfile ParticleSystem_lifetimeVariation.dot
    auto_ptr< SFFloat > lifetimeVariation;

    /// The maximum number of particles to be generated.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 200
    /// 
    /// \dotfile ParticleSystem_maxParticles.dot
    auto_ptr< SFInt32 > maxParticles;

    /// \brief The particleLifetime field controls the lifetime of a particle
    /// i.e. the time in seconds the particle exists from the point
    /// it is being emitted. See also the lifetimeVariation field.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 5
    /// 
    /// \dotfile ParticleSystem_particleLifetime.dot
    auto_ptr< SFFloat > particleLifetime;

    /// The particleSize field describes the dimensions in metres of the width
    /// and height of each particle. Changing this value dynamically will only
    /// change new particles created after the change. Particles created before
    /// this timestamp will remain at the old size. This field only effects
    /// particles using "LINE", "QUAD", "SPRITE", and "TRIANGLE" geometry 
    /// types.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0.02 0.02
    /// 
    /// \dotfile ParticleSystem_particleSize.dot
    auto_ptr< SFVec2f > particleSize;

    /// The isActive field indicates whether the particle system is
    /// currently running, based on the setup of the node.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile ParticleSystem_isActive.dot
    auto_ptr< SFBool > isActive;

    /// The colorRamp field holds a series of colour values to be used at
    /// the given colorKey points in time for the particle over its lifetime.
    /// Between keys, colour values are interpreted in a linear HSV 
    /// space, using the same rules defined for the ColorInterpolator node. 
    /// The colour values are defined as per-vertex colour values. 
    /// Consequently, if an appearance node with material is provided, the
    /// material properties will override the colour ramp.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_colorRamp.dot
    auto_ptr< SFColorNode > colorRamp;

    /// The colorKey field represents the time of the particle in seconds and 
    /// are the keys to use for the colorRamp field.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_colorKey.dot
    auto_ptr< MFFloat > colorKey;

    /// The emitter field specifies the type of emitter geometry and 
    /// properties that the particles are given for their initial positions.
    /// After being created, the individual particles are then manipulated
    /// according to the physics model(s) specified in the physics field.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_emitter.dot
    auto_ptr< SFEmitterNode > emitter;

    /// The geometryType field specifies the type of geometry that should be 
    /// used to represent individual particles. Typically, a particle is 
    /// calculated as a point in space at which the geometry is placed and then
    /// rendered using the appearance attributes.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "QUAD"
    /// <b>Allowed values:</b> "LINE", "POINT", "QUAD", "SPRITE", 
    /// "TRIANGLE", "GEOMETRY"
    /// 
    /// \dotfile ParticleSystem_geometryType.dot
    auto_ptr< SFString > geometryType;

    /// \brief The physics field contains the physics models that should affect
    /// the particles after they have been created, e.g. gravity and wind
    /// effects.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_physics.dot
    auto_ptr< MFPhysicsModelNode > physics;

    /// The texCoordRamp fields control the texture coordinates of the
    /// provided texture(s) in the Appearance node, over time.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_texCoordRamp.dot
    auto_ptr< SFTextureCoordinate > texCoordRamp;

    /// The colorKey field represents the time of the particle in seconds and 
    /// are the keys to use for the texCoordRamp field.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ParticleSystem_texCoordKey.dot
    auto_ptr< MFFloat > texCoordKey;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    typedef std::list< X3DParticleEmitterNode::Particle > Particles;

    typedef list< ParticleSystem * > ParticleSystemList;

    /// Returns a list of all current ParticleSystem instances.
    static const ParticleSystemList &getAllParticleSystems() {
      return particlesystems;
    }

  protected:
    H3DTime particle_system_time;
    H3DTime last_time;
    Particles particles;
    bool update_bound;
    static ParticleSystemList particlesystems;
  };
}

#endif
