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
/// \file Sound.h
/// \brief Header file for Sound, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SOUND_H__
#define __SOUND_H__


#include <H3D/X3DSoundNode.h>
#include <H3D/X3DSoundSourceNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Sound
  /// \brief The Sound node specifies the spatial presentation of a sound in
  /// a X3D scene. 
  ///
  /// The sound is located at a point in the local coordinate system
  /// and emits sound in an elliptical pattern (defined by two
  /// ellipsoids). The ellipsoids are oriented in a direction specified by
  /// the direction field. The shape of the ellipsoids may be modified to
  /// provide more or less directional focus from the location of the
  /// sound. 
  ///
  /// The source field specifies the sound source for the Sound node. If the
  /// source field is not specified, the Sound node will not emit audio. The
  /// source field shall specify an AudioClip node . 
  ///
  ///  The intensity field adjusts the loudness (decibels) of the sound
  /// emitted by the Sound node. The intensity field has a value that ranges
  /// from 0.0 to 1.0 and specifies a factor which shall be used to scale the
  /// normalized sample data of the sound source during playback. A Sound node
  /// with an intensity of 1.0 shall emit audio at its maximum loudness 
  /// (before attenuation), and a Sound node with an intensity of 0.0 shall
  /// emit no audio. Between these values, the loudness should increase 
  /// linearly from a -20 dB change approaching an intensity of 0.0 to a 
  /// 0 dB change at an intensity of 1.0.
  ///
  /// The priority field provides a hint for the browser to choose which 
  /// sounds to play when there are more active Sound nodes than can be played
  /// at once due to either limited system resources or system load. The
  /// priority field ranges from 0.0 to 1.0, with 1.0 being the highest 
  /// priority and 0.0 the lowest priority.
  ///
  /// The location field determines the location of the sound emitter in the
  /// local coordinate system. A Sound node's output is audible only if it is
  /// part of the traversed scene. Sound nodes that are descended from LOD, 
  /// Switch, or any grouping or prototype node that disables traversal 
  /// of its children are not audible unless they are traversed. If a Sound 
  /// node is disabled by a Switch or LOD node, and later it becomes part of
  /// the traversal again, the sound shall resume where it would have been
  /// had it been playing continuously.
  ///
  /// The Sound node has an inner ellipsoid that defines a volume of space 
  /// in which the maximum level of the sound is audible. Within this 
  /// ellipsoid, the normalized sample data is scaled by the intensity field 
  /// and there is no attenuation. The inner ellipsoid is defined by extending
  /// the direction vector through the location. The minBack and minFront
  /// fields specify distances behind and in front of the location along the
  /// direction vector respectively. The inner ellipsoid has one of its foci
  /// at location (the second focus is implicit) and intersects the direction
  /// vector at minBack and minFront.
  ///
  /// The Sound node has an outer ellipsoid that defines a volume of space
  /// that bounds the audibility of the sound. No sound can be heard outside
  /// of this outer ellipsoid. The outer ellipsoid is defined by extending
  /// the direction vector through the location. The maxBack and maxFront 
  /// fields specify distances behind and in front of the location along the
  /// direction vector respectively. The outer ellipsoid has one of its foci
  /// at location (the second focus is implicit) and intersects the direction 
  /// vector at maxBack and maxFront.
  ///
  /// The minFront, maxFront, minBack, and maxBack fields are defined in local
  /// coordinates, and shall be greater than or equal to zero. The minBack
  /// field shall be less than or equal to maxBack, and minFront shall be less
  /// than or equal to maxFront. The ellipsoid parameters are specified in the
  /// local coordinate system but the ellipsoids' geometry is affected by 
  /// ancestors' transformations.
  ///
  /// Between the two ellipsoids, there shall be a linear attenuation ramp in
  /// loudness, from 0 dB at the minimum ellipsoid to -20 dB at the maximum
  /// ellipsoid:
  ///
  ///   attenuation = -20 * (d' / d'')
  ///
  /// where d' is the distance along the location-to-viewer vector, measured 
  /// from the transformed minimum ellipsoid boundary to the viewer, and d'' 
  /// is the distance along the location-to-viewer vector from the transformed
  /// minimum ellipsoid boundary to the transformed maximum ellipsoid boundary.
  /// 
  /// The spatialize field specifies if the sound is perceived as being
  /// directionally located relative to the viewer. If the spatialize field
  /// is TRUE and the viewer is located between the transformed inner and outer
  /// ellipsoids, the viewer's direction and the relative location of the
  /// Sound node should be taken into account during playback. If the
  /// spatialize field is FALSE, directional effects are ignored, but the 
  /// ellipsoid dimensions and intensity will still affect the loudness of
  /// the sound. If the sound source is multi-channel, the source shall retain
  /// its channel separation during playback.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Sound.x3d">Sound.x3d</a>
  ///     ( <a href="examples/Sound.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Sound.dot
  class H3DAPI_API Sound : public X3DSoundNode {
  public:
    
    /// The SFSoundSourceNode registers the Sound node in the 
    /// X3DSoundSourceNode that is put into the field.
    class H3DAPI_API SFSoundSourceNode: public TypedSFNode< X3DSoundSourceNode > {
    public:
      /// Destructor.
      ~SFSoundSourceNode() { value = NULL; }
    protected:
      virtual void onAdd( Node *n );
      virtual void onRemove( Node *n );
    };
    
    /// Field that calls ALrender() when a field routed to it
    /// has generated an event.
    class ALSoundSetup: public PeriodicUpdate< Field > {
    public:
      virtual void update() {
        Sound *s = 
          static_cast< Sound * >( getOwner());
        s->ALrender();
      }
    };

    /// Constructor.
    Sound( Inst< SFNode >  _metadata   = 0,
           Inst< SFVec3f>  _direction  = 0,
           Inst< SFFloat>  _intensity  = 0,
           Inst< SFVec3f>  _location   = 0,
           Inst< SFFloat>  _maxBack    = 0,
           Inst< SFFloat>  _maxFront   = 0,
           Inst< SFFloat>  _minBack    = 0,
           Inst< SFFloat>  _minFront   = 0,
           Inst< SFFloat>  _priority   = 0,
           Inst< SFSoundSourceNode >  _source     = 0,
           Inst< SFBool >  _spatialize = 0,
           Inst< ALSoundSetup > _soundSetup = 0 );

    ~Sound();

    /// Traverse the scenegraph. Saves the accForwardMatrix and calls
    /// traverseSG() for source.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// Perform the OpenAL calls to render the sound node.
    virtual void ALrender(); 

    /// The direction field specifies the direction of the ellipsoids.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 1 ) \n
    /// 
    /// \dotfile Sound_direction.dot
    auto_ptr< SFVec3f >  direction;

    /// The intensity field adjusts the loudness (decibels) of the sound 
    /// emitted by the Sound node. The intensity field has a value that
    /// ranges from 0.0 to 1.0 and specifies a factor which shall be used
    /// to scale the normalized sample data of the sound source during
    /// playback. A Sound node with an intensity of 1.0 shall emit audio at
    /// its maximum loudness (before attenuation), and a Sound node with an
    /// intensity of 0.0 shall emit no audio. Between these values, the 
    /// loudness should increase linearly from a -20 dB change approaching
    /// an intensity of 0.0 to a 0 dB change at an intensity of 1.0.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Sound_intensity.dot
    auto_ptr< SFFloat >  intensity;

    /// The location field determines the location of the sound emitter in 
    /// the local coordinate system.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile Sound_location.dot
    auto_ptr< SFVec3f >  location;

    /// The maxBack field specifies the back value of the outer ellipsoid. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 10 \n
    /// 
    /// \dotfile Sound_maxBack.dot
    auto_ptr< SFFloat >  maxBack;

    /// The maxFront field specifies the front value of the outer ellipsoid. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 10 \n
    /// 
    /// \dotfile Sound_maxFront.dot
    auto_ptr< SFFloat >  maxFront;

    /// The minBack field specifies the back value of the inner ellipsoid. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Sound_minBack.dot
    auto_ptr< SFFloat >  minBack;

    /// The minFront field specifies the front value of the inner ellipsoid. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Sound_minFront.dot
    auto_ptr< SFFloat >  minFront;

    /// The priority field provides a hint for the browser to choose which
    /// sounds to play when there are more active Sound nodes than can be
    /// played at once due to either limited system resources or system load. 
    /// The priority field ranges from 0.0 to 1.0, with 1.0 being the highest
    /// priority and 0.0 the lowest priority.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile Sound_priority.dot
    auto_ptr< SFFloat >  priority;

    /// The source field specifies the sound source for the Sound node. If
    /// the source field is not specified, the Sound node will not emit audio.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile Sound_source.dot
    auto_ptr< SFSoundSourceNode  >  source;

    /// The spatialize field specifies if the sound is perceived as being 
    /// directionally located relative to the viewer. If the spatialize
    /// field is TRUE and the viewer is located between the transformed
    /// inner and outer ellipsoids, the viewer's direction and the relative 
    /// location of the Sound node should be taken into account during 
    /// playback.  If the spatialize field is FALSE, directional effects are 
    /// ignored, but the ellipsoid dimensions and intensity will still affect
    /// the loudness of the sound. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Sound_spatialize.dot
    auto_ptr< SFBool  >  spatialize;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    public:
#ifdef HAVE_OPENAL
    static bool alut_initialized;
    static ALCdevice *al_device;
    static ALCcontext *al_context;
#endif
    /// Field that calls ALrender() when a field routed to it
    /// has generated an event.
    auto_ptr< ALSoundSetup > soundSetup;

    /// The matrix from local space to global space.
    auto_ptr< SFMatrix4f > accForwardMatrix;
  };
}

#endif
