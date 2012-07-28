/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QPlasmaUtils.h"
#include <ResManager/pdUnifiedTypeMap.h>
#include <PRP/Object/plSceneObject.h>

bool s_showTypeIDs = false;

enum {
    kIcoSceneObj, kIcoSceneNode, kIcoDraw, kIcoDraw2, kIcoSim, kIcoCoord,
    kIcoSound, kIcoLayer, kIcoMaterial, kIcoImg, kIcoPython, kIcoGUIButton,
    kIcoGUICheck, kIcoGUIClick, kIcoGUIDialog, kIcoGUIEdit, kIcoGUIEdit2,
    kIcoGUIListBox, kIcoGUIProgress, kIcoGUIText, kIcoGUIUpDown,
    kIcoGUIRadioGroup, kIcoGUIKnob, kIcoSynch, kNumIcons
};

QIcon pqGetTypeIcon(short classType)
{
    static QIcon s_icons[kNumIcons] = {
        QIcon(":/img/sceneobj.png"), QIcon(":/img/scenenode.png"),
        QIcon(":/img/drawspans.png"), QIcon(":/img/drawspans2.png"),
        QIcon(":/img/sim.png"), QIcon(":/img/coords.png"),
        QIcon(":/img/snd.png"), QIcon(":/img/layer.png"),
        QIcon(":/img/material.png"), QIcon(":/img/img.png"),
        QIcon(":/img/py.png"), QIcon(":/img/gui_button.png"),
        QIcon(":/img/gui_check.png"), QIcon(":/img/gui_click.png"),
        QIcon(":/img/gui_dialog.png"), QIcon(":/img/gui_edit.png"),
        QIcon(":/img/gui_edit2.png"), QIcon(":/img/gui_listbox.png"),
        QIcon(":/img/gui_progress.png"), QIcon(":/img/gui_text.png"),
        QIcon(":/img/gui_updown.png"), QIcon(":/img/gui_radiogroup.png"),
        QIcon(":/img/gui_knob.png"), QIcon(":/img/synch.png"),
    };

    switch (classType & ~0x3000) {
    case kSceneNode:
        return s_icons[kIcoSceneNode];
    case kSceneObject:
        return s_icons[kIcoSceneObj];
    case kMipmap:
        return s_icons[kIcoImg];
    case kCubicEnvironmap:
        return s_icons[kIcoImg];
    case kCoordinateInterface:
        return s_icons[kIcoCoord];
    case kDynamicTextMap:
        return s_icons[kIcoGUIText];
    case kSimulationInterface:
        return s_icons[kIcoSim];
    case kAudioInterface:
        return s_icons[kIcoSound];
    case kDrawInterface:
        return s_icons[kIcoDraw2];
    case kDrawableSpans:
        return s_icons[kIcoDraw];
    case kLayer:
        return s_icons[kIcoLayer];
    case kLayerAnimation:
        return s_icons[kIcoLayer];
    case kLayerDepth:
        return s_icons[kIcoLayer];
    case kLayerSDLAnimation:
        return s_icons[kIcoLayer];
    case kGMaterial:
        return s_icons[kIcoMaterial];
    case kSoundBuffer:
        return s_icons[kIcoSound];
    case kPythonFileMod:
        return s_icons[kIcoPython];
    case kGUIButtonMod:
        return s_icons[kIcoGUIButton];
    case kGUICheckBoxCtrl:
        return s_icons[kIcoGUICheck];
    case kGUIClickMapCtrl:
        return s_icons[kIcoGUIClick];
    case kGUIDialogMod:
        return s_icons[kIcoGUIDialog];
    case kGUIEditBoxMod:
        return s_icons[kIcoGUIEdit];
    case kGUIKnobCtrl:
        return s_icons[kIcoGUIKnob];
    case kGUIListBoxMod:
        return s_icons[kIcoGUIListBox];
    case kGUIMultiLineEditCtrl:
        return s_icons[kIcoGUIEdit2];
    case kGUIProgressCtrl:
        return s_icons[kIcoGUIProgress];
    case kGUITextBoxMod:
        return s_icons[kIcoGUIText];
    case kGUIUpDownPairMod:
        return s_icons[kIcoGUIUpDown];
    case kGUIRadioGroupCtrl:
        return s_icons[kIcoGUIRadioGroup];
    case kSynchedObject:
        return s_icons[kIcoSynch];
    case kMsgForwarder:
        return s_icons[kIcoSynch];
    case kSound:
        return s_icons[kIcoSound];
    case kWin32Sound:
        return s_icons[kIcoSound];
    case kWin32StaticSound:
        return s_icons[kIcoSound];
    case kWin32StreamingSound:
        return s_icons[kIcoSound];
    case kWin32GroupedSound:
        return s_icons[kIcoSound];
    case kWin32LinkSound:
        return s_icons[kIcoSound];
    default:
        return QIcon();
    }
}

QString pqGetFriendlyClassName(short classType)
{
    static QString S_INVALID = "(INVALID)";
    static QString s_names[TYPESPACE_MAX] = {
        "Scene Node", "Scene Object", "Keyed Object", "Bitmap", "Mipmap",
        "Cubic Environmap", "Layer", "Material", "Particle System",
        "Particle Effect", "Particle Collision Effect Beat",
        "Particle Fade Volume Effect", "Bound Interface", "Render Target",
        "Cubic Render Target", "Cubic Render Target Modifier", "Obj Interface",
        "Audio Interface", "Audible", "Audible (Null)", "Win Audible",
        "Coordinate Interface", "Draw Interface", "Drawable", "Drawable Mesh",
        "Drawable Ice", "Physical", "Physical Mesh", "Simulation Interface",
        "Camera Modifier", "Modifier", "Single-Modifier", "Simple Modifier",
        "Simple TM Modifier", "Random TM Modifier", "Interesting Modifier",
        "Detector Modifier", "Simple Physical Mesh", "Compound Physical Mesh",
        "Multi-Modifier", "Synched Object", "Sound Buffer", "Alias Modifier",
        "Picking Detector", "Collision Detector", "Logic Modifier",
        "Conditional Object", "AND Conditional Object", "OR Conditional Object",
        "Picked Conditional Object", "Activator Conditional Object",
        "Timer Callback Manager", "Key-Press Conditional Object",
        "Animation Event Conditional Object", "Control Event Conditional Object",
        "Object-In-Box Conditional Object",
        "Local Player-In-Box Conditional Object",
        "Object-Intersect-Plane Conditional Object",
        "Local Player-Intersect-Plane Conditional Object", "Portal Drawable",
        "Portal Physical (The cake is a lie)", "Spawn Modifier",
        "Facing Conditional Object", "Generic Physical (PX, HK, ODE)",
        "View Face Modifier", "Layer Interface", "Layer Wrapper",
        "Layer Animation", "Layer Depth", "Layer Movie", "Layer Bink",
        "Layer AVI", "Sound", "Win32 Sound", "Layer OR", "Audio System",
        "Drawable Spans", "Drawable Patch Set", "Input Manager",
        "Logic Mod Base", "Fog Environment", "Net App", "Net Client Mgr",
        "2-Way Win Audible", "Light Info", "Directional Light Info",
        "Omni Light Info", "Spot Light Info", "Light Space", "Net Client App",
        "Net Server App", "Client", "Compound TM Modifier", "Camera Brain",
        "Camera Brain (Default)", "Camera Brain (Drive)", "Camera Brain (Fixed)",
        "Camera Brain (Fixed-Pan)", "GUI Click Map Ctrl", "Listener",
        "Avatar Mod", "Avatar Anim", "Avatar Anim Mgr", "Occluder",
        "Mobile Occluder", "Layer Shadow Base", "Limited Dir Light Info",
        "AG Anim", "AG Modifier", "AG Master Mod", "Camera Brain (Avatar)",
        "Camera Region Detector", "Camera Brain (First-Person)",
        "Line Follow Mod", "Light Modifier", "Omni Modifier", "Spot Modifier",
        "Limited Dir Modifier", "Seek Point Mod", "One Shot Mod",
        "Random Command Mod", "Random Sound Mod", "Post Effect Mod",
        "Object-In-Volume Detector", "Responder Modifier", "Axis Anim Modifier",
        "Layer Light Base", "Follow Mod", "Transition Mgr", "Inventory Mod",
        "Inventory Obj Mod", "Link Effects Mgr", "Win32 Streaming Sound",
        "Python Mod", "Activator Activator Conditional Object", "Soft Volume",
        "Soft Volume Simple", "Soft Volume Complex", "Soft Volume Union",
        "Soft Volume Intersect", "Soft Volume Invert", "Win32 Link Sound",
        "Layer Link Animation", "Armature Mod", "Camera Brain (Freelook)",
        "Havok Constraints Mod", "Hinge Constraint Mod", "Wheel Constraint Mod",
        "Strong Spring Constraint Mod", "Armature LOD Mod", "Win32 Static Sound",
        "Game GUI Mgr", "GUI Dialog Mod", "Camera Brain 1", "Virtual Cam 1",
        S_INVALID, "Camera Brain 1 (Drive)", "Camera Brain 1 (POA)",
        "Camera Brain 1 (Avatar)", "Camera Brain 1 (Fixed)",
        "Camera Brain 1 (POA-Fixed)", "GUI Button Mod", "Python File Mod",
        "GUI Control Mod", "Exclude Region Modifier", "GUI Draggable Mod",
        "Volume Sensor Conditional Object", "Vol Activator Conditional Object",
        "Msg Forwarder", "Blower", "GUI List Box Mod", "GUI Text Box Mod",
        "GUI Edit Box Mod", "Dynamic Text Map", "Sitting Modifier",
        "GUI Up-Down Pair Mod", "GUI Value Ctrl", "GUI Knob Ctrl",
        "Av Ladder Mod", "Camera Brain 1 (First Person)", "Clone Spawn Modifier",
        "Clothing Item", "Clothing Outfit", "Clothing Base", "Clothing Mgr",
        "GUI Drag Bar Ctrl", "GUI Check Box Ctrl", "GUI Radio Group Ctrl",
        "Player Book Mod", "GUI Dyn Display Ctrl", "Layer Project",
        "Input Interface Mgr", "Rail Camera Mod", "Multistage Beh Mod",
        "Camera Brain 1 (Circle)", "Particle Wind Effect", "Anim Event Modifier",
        "Auto Profile", "GUI Skin", "AVI Writer", "Particle Collision Effect",
        "Particle Collision Effect Die", "Particle Collision Effect Bounce",
        "Interface Info Modifier", "Shared Mesh", "Armature Effects Mgr",
        "Marker Mgr", "Vehicle Modifier", "Particle Local Wind",
        "Particle Uniform Wind", "Instance Draw Interface", "Shadow Master",
        "Shadow Caster", "Point Shadow Master", "Direct Shadow Master",
        "SDL Modifier", "Physical SDL Modifier", "Clothing SDL Modifier",
        "Avatar SDL Modifier", "AG Master SDL Modifier", "Python SDL Modifier",
        "Layer SDL Modifier", "Anim Time Convert SDL Modifier",
        "Responder SDL Modifier", "Sound SDL Modifier", "Res Manager Helper",
        "HK Sub-World", "Armature Effect", "Armature Effect Foot Sound",
        "EAX Listener Mod", "Dyna Decal Mgr",
        "Object In Volume And Facing Detector", "Dyna Foot Mgr",
        "Dyna Ripple Mgr", "Dyna Bullet Mgr", "Decal Enable Mod", "Print Shape",
        "Dyna Puddle Mgr", "GUI Multi-Line Edit Ctrl", "Layer Animation Base",
        "Layer SDL Animation", "ATC Anim", "Age-Global Anim",
        "Subworld Region Detector", "Avatar Mgr", "NPC Spawn Mod",
        "Active Print Shape", "Exclude Region SDL Modifier",
        "LOS Dispatch", "Dyna Wake Mgr", "Simulation Mgr", "Wave Set 7",
        "Panic Link Region", "Win32 Grouped Sound", "Filter Coord Interface",
        "Stereizer", "CCR Mgr", "CCR Specialist", "CCR Senior Specialist",
        "CCR Shift Supervisor", "CCR Game Operator", "Shader", "Dynamic Env Map",
        "Simple Region Sensor", "Morph Sequence", "Emote Anim",
        "Dyna Ripple VS Mgr", "Wave Set 6", "GUI Progress Ctrl",
        "Maintainer's Marker Modifier", "Morph Sequence SDL Mod",
        "Morph Data Set", "Hard Region", "Hard Region Planes",
        "Hard Region Complex", "Hard Region Union", "Hard Region Intersect",
        "Hard Region Invert", "Vis Region", "Vis Mgr", "Region Base",
        "GUI Pop-Up Menu", "GUI Menu Item", "Coop Coordinator", "Font",
        "Font Cache", "Relevance Region", "Relevance Mgr", "Journal Book",
        "Layer Target Container", "Image Lib Mod", "Particle Flock Effect",
        "Particle SDL Mod", "Age Loader", "Wave Set Base", "Physical Snd Group",
        "Book Data", "Dyna Torpedo Mgr", "Dyna Torpedo VS Mgr", "Cluster Group",
        "Game Marker Modifier", "LOD Mipmap", "Swim Detector",
        "Fade Opacity Mod", "Fade Opacity Layer", "Dist Opacity Mod",
        "Armature Mod Base", "Swim Region Interface",
        "Swim Circular Current Region", "Particle Follow System Effect",
        "Swim Straight Current Region", "Object Flocker", "Grass Shader Mod",
        "Dynamic Cam Map", S_INVALID, S_INVALID, S_INVALID, "Auto-Walk Region",
        "Crossfade", "Particle Fade-Out Effect", "Secure Preloader",
        "Wind Bone Mod", "Camera Brain (Novice-Plus)", "Subtitle Mgr",
        "Python File Mod Conditional Object", "Layer Transform",
        "Bubble Shader Mod", "Line Follow Mod Base", "Client App",
        "GUI Credits Ctrl", "Camera Brain Uru", "Virtual Camera",
        "Camera Brain Uru (Drive)", "Camera Brain Uru (Follow)",
        "Camera Brain Uru (Fixed)", "GUI Sketch Ctrl", "Ladder Modifier",
        "Camera Brain Uru (First-Person)", "Camera Brain Uru (Circle)",
        "EAX Reverb Effect", "Spawn Mod", "Camera Brain (Novice)",
        "Avatar Physical SDL Modifier", "Direct Music Sound",
        "Client Session Mgr", "SDL Var Change Notifier",
        "Interest Well Modifier", "Elevator Modifier", "Camera Brain (Expert)",
        "Paging Region Modifier", "Guidepath Modifier", "Node Mgr", "EAX Effect",
        "EAX Pitch Shifter", "IK Modifier", "Camera Brain (M5)", "AG Anim Bink",
        "Tree Shader", "Node Region Modifier", "Piranha Region Modifier",
        "Riding Animated Physical Detector",
        "Volume Sensor Conditional Object No Arbitration", "FX Material",
        "Movable Mod", "Material", "Effect", "Particle Bullet Effect",
        "Camera Brain (Ground)", "Camera Brain (Flight)", S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID,

        "Obj Ref Msg", "Node Ref Msg", "Message", "Ref Msg", "General Ref Msg",
        "Time Msg", "Anim Cmd Msg", "Particle Update Msg", "Layer Ref Msg",
        "Material Ref Msg", "Camera Msg", "Input Event Msg", "Key Event Msg",
        "Mouse Event Msg", "Eval Msg", "Transform Msg", "Control Event Msg",
        "Vault CCR Node", "LOS Request Msg", "LOS Hit Msg", "Single Mod Msg",
        "Multi Mod Msg", "Avatar Physics Enable Callback Msg",
        "Member Update Msg", "Net Msg Paging Room", "Activator Msg", "Dispatch",
        "Receiver", "Mesh Ref Msg", "Render Procs", "Sfx Angle Fade",
        "Sfx Dist Fade", "Sfx Dist Shade", "Sfx Global Shade",
        "Sfx Intense Alpha", "Sfx Obj Dist Fade", "Sfx Obj Dist Shade",
        "Dynamic Value", "Dynamic Scalar", "Dynamic ColorRGBA",
        "Dynamic Matrix33", "Dynamic Matrix44", "Controller",
        "MOUL/Myst 5 Controller", "Scale Controller", "Rot Controller",
        "Pos Controller", "Scalar Controller", "Point3 Controller",
        "Scale-Value Controller", "Quat Controller", "Matrix33 Controller",
        "Matrix44 Controller", "Ease Controller", "Simple Scale Controller",
        "Simple Rot Controller", "Compound Rot Controller",
        "Simple Pos Controller", "Compound Pos Controller", "TM Controller",
        "Fog Control", "Int Ref Msg", "Collision Reactor", "Correction Msg",
        "Physical Modifier", "Picked Msg", "Collide Msg", "Trigger Msg",
        "Interesting Mod Msg", "Debug Key Event Msg", "Physical Properties",
        "Simple Phys", "Matrix Update Msg", "Cond Ref Msg", "Timer Callback Msg",
        "Event Callback Msg", "Spawn Mod Msg", "Spawn Request Msg",
        "Load Clone Msg", "Enable Msg", "Warp Msg", "Attach Msg", "Console",
        "Render Msg", "Anim Time Convert", "Sound Msg", "Interesting Ping",
        "Node Cleanup Msg", "Space Tree", "Net Message", "Net Msg Join Request",
        "Net Msg Join Ack", "Net Msg Leave", "Net Msg Ping",
        "Net Msg Rooms List", "Net Msg Group Owner",
        "Net Msg Game State Request", "Net Msg Session Reset", "Net Msg Omnibus",
        "Net Msg Object", "CCR Invisible Msg", "Link In Done Msg",
        "Net Msg Game Message", "Net Msg Stream", "Audio Sys Msg",
        "Dispatch Base", "Server Reply Msg", "Device Recreate Msg",
        "Net Msg Stream Helper", "Net Msg Object Helper", "IMouse X-Event Msg",
        "IMouse Y-Event Msg", "IMouse B-Event Msg", "Logic Trigger Msg",
        "Pipeline", "DX8 Pipeline", "Net Msg Voice", "Light Ref Msg",
        "Net Msg Streamed Object", "Net Msg Shared State",
        "Net Msg Test And Set", "Net Msg Get Shared State", "Shared State Msg",
        "Net Generic Server Task",
        "Net Lookup Server Get Age Info From Vault Task", "Load Age Msg",
        "Message With Callbacks", "Client Msg", "Client Ref Msg",
        "Net Msg Obj State Request", "CCR Petition Msg",
        "Vault CCR Initialization Task", "Net Server Msg",
        "Net Server Msg With Context", "Net Server Msg Register Server",
        "Net Server Msg Unregister Server", "Net Server Msg Start Process",
        "Net Server Msg Register Process", "Net Server Msg Unregister Process",
        "Net Server Msg Find Process", "Net Server Msg Process Found",
        "Net Msg Routing Info", "Net Server Session Info", "Simulation Msg",
        "Simulation Synch Msg", "HK Simulation Synch Msg", "Avatar Msg",
        "Av Task Msg", "Av Seek Msg", "Av One-Shot Msg", "Satisfied Msg",
        "Net Msg Object List Helper", "Net Msg Object Update Filter",
        "Proxy Draw Msg", "Self-Destruct Msg", "Sim Influence Msg", "Force Msg",
        "Offset Force Msg", "Torque Msg", "Impulse Msg", "Offset Impulse Msg",
        "Angular Impulse Msg", "Damp Msg", "Shift Mass Msg", "Sim State Msg",
        "Freeze Msg", "Event Group Msg", "Suspend Event Msg",
        "Net Msg Members List Req", "Net Msg Members List",
        "Net Msg Member Info Helper", "Net Msg Member List Helper",
        "Net Msg Member Update", "Net Msg Server To Client",
        "Net Msg Create Player", "Net Msg Authenticate Hello",
        "Net Msg Authenticate Challenge", "Connected To Vault Msg",
        "CCR Communication Msg", "Net Msg Initial Age State Sent",
        "Initial Age StateLoaded Msg", "Net Server Msg Find Server Base",
        "Net Server Msg Find Server Reply Base",
        "Net Server Msg Find Auth Server",
        "Net Server Msg Find Auth Server Reply",
        "Net Server Msg Find Vault Server",
        "Net Server Msg Find Vault Server Reply", "Av Task Seek Done Msg",
        "Avatar Spawn Notify Msg", "Net Server Msg Vault Task",
        "Net Msg Vault Task", "Age Link Struct", "Vault Age Info Node",
        "Net Msg Streamable Helper", "Net Msg Receivers List Helper",
        "Net Msg Listen List Update", "Net Server Msg Ping", "Net Msg Alive",
        "Net Msg Terminated", "SDL Modifier Msg", "Net Msg SDL State",
        "Net Server Msg Session Reset", "CCR Ban Linking Msg",
        "CCR Silence Player Msg", "Render Request Msg", "Render Request Ack",
        "Net Member", "Net Game Member", "Net Transport Member", "Convex Volume",
        "Particle Generator", "Simple Particle Generator", "Particle Emitter",
        "AG Channel", "Matrix Channel", "Matrix Time Scale", "Matrix Blend",
        "Matrix Controller Channel", "Quat Point Combine", "Point Channel",
        "Point Constant", "Point Blend", "Quat Channel", "Quat Constant",
        "Quat Blend", "Link To Age Msg", "Player Page Msg", "Cmd Iface Mod Msg",
        "Net Server Msg Pls Update Player", "Listener Msg", "Anim Path",
        "Clothing Update BC Msg", "Notify Msg", "Fake Out Msg",
        "Cursor Change Msg", "Node Change Msg", "Av Enable Msg",
        "Link Callback Msg", "Transition Msg", "Console Msg", "Volume Isect",
        "Sphere Isect", "Cone Isect", "Cylinder Isect", "Parallel Isect",
        "Convex Isect", "Complex Isect", "Union Isect", "Intersection Isect",
        "Modulator", "Inventory Msg", "Link Effects Trigger Msg",
        "Link Effect BC Msg", "Responder Enable Msg", "Net Server Msg Hello",
        "Net Server Msg Hello Reply", "Net Server Member", "Responder Msg",
        "One-Shot Msg", "Vault Age Info List Node",
        "Net Server Msg Server Registered", "Point Time Scale",
        "Point Controller Channel", "Quat Time Scale", "AG Applicator",
        "Matrix Channel Applicator", "Point Channel Applicator",
        "Light Diffuse Applicator", "Light Ambient Applicator",
        "Light Specular Applicator", "Omni Applicator",
        "Quat Channel Applicator", "Scalar Channel", "Scalar Time Scale",
        "Scalar Blend", "Scalar Controller Channel", "Scalar Channel Applicator",
        "Spot Inner Applicator", "Spot Outer Applicator",
        "Net Server Msg Pls Routable Msg", "Puppet Brain Msg", "ATC Ease Curve",
        "Const Accel Ease Curve", "Spline Ease Curve",
        "Vault Age Info Initialization Task", "Game GUI Msg",
        "Net Server Msg Vault Request Game State",
        "Net Server Msg Vault Game State",
        "Net Server Msg Vault Game State Save",
        "Net Server Msg Vault Game State Saved",
        "Net Server Msg Vault Game State Load", "Net Client Task",
        "Net Msg SDL State BCast", "Replace Geometry Msg",
        "Net Server Msg Exit Process", "Net Server Msg Save Game State",
        "D'ni Coordinate Info", "Net Msg Game Message Directed",
        "Link-Out Unload Msg", "Scalar Constant", "Matrix Constant",
        "AG Cmd Msg", "Particle Transfer Msg", "Particle Kill Msg",
        "Exclude Region Msg", "One-Time Particle Generator",
        "Particle Applicator", "Particle Life Min Applicator",
        "Particle Life Max Applicator", "Particle PPS Applicator",
        "Particle Angle Applicator", "Particle Vel Min Applicator",
        "Particle Vel Max Applicator", "Particle Scale Min Applicator",
        "Particle Scale Max Applicator", "Dynamic Text Msg",
        "Camera Target Fade Msg", "Age Loaded Msg",
        "Point Controller Cache Channel", "Scalar Controller Cache Channel",
        "Link Effects Trigger Prep Msg", "Link Effect Prep BC Msg",
        "Avatar Input State Msg", "Age Info Struct", "SDL Notification Msg",
        "Net Client Connect Age Vault Task", "Linking Mgr Msg",
        "Vault Notify Msg", "Player Info", "Swap Spans Ref Msg", "KI",
        "DI Spans Msg", "Net Msg Creatable Helper", "Server Guid",
        "Net Msg Request My Vault Player List", "Delayed Transform Msg",
        "Super V-Node Mgr Init Task", "Element Ref Msg", "Clothing Msg",
        "Event Group Enable Msg", "GUI Notify Msg", "Av Brain", "Armature Brain",
        "Av Brain Human", "Av Brain Critter", "Av Brain Drive",
        "Av Brain Sample", "Av Brain Generic", "Av Brain Puppet",
        "Av Brain Ladder", "Input Iface Mgr Msg", "KI Msg",
        "Remote Avatar Info Msg", "Matrix Delayed Correction Applicator",
        "Av Push Brain Msg", "Av Pop Brain Msg", "Room Load Notify Msg",
        "Av Task", "Av Anim Task", "Av Seek Task", "Av Blended Seek Task",
        "Av One-Shot Task", "Av Enable Task", "Av Task Brain", "Anim Stage",
        "Net Client Member", "Net Client Comm Task",
        "Net Server Msg Auth Request", "Net Server Msg Auth Reply",
        "Net Client Comm Auth Task", "Client Guid", "Net Msg Vault Player List",
        "Net Msg Set My Active Player",
        "Net Server Msg Request Account Player List",
        "Net Server Msg Account Player List", "Net Msg Player Created",
        "Net Server Msg Vault Create Player",
        "Net Server Msg Vault Player Created", "Net Msg Find Age",
        "Net Msg Find Age Reply", "Net Client Connect Prep Task",
        "Net Client Auth Task", "Net Client Get Player Vault Task",
        "Net Client Set Active Player Task", "Net Client Find Age Task",
        "Net Client Leave Task", "Net Client Join Task",
        "Net Client Calibrate Task", "Net Msg Delete Player",
        "Net Server Msg Vault Delete Player", "Net Core Stats Summary",
        "Creatable Generic Value", "Creatable List Helper", "Creatable Stream",
        "Av Brain Generic Msg", "Av Task Seek", "AG Instance Callback Msg",
        "Armature Effect Msg", "Armature Effect State Msg", "Shadow Cast Msg",
        "Bounds Isect", "Net Client Comm Leave Task", "Res Mgr Helper Msg",
        "Net Msg Authenticate Response", "Net Msg Account Authenticated",
        "Net Client Comm Send Periodic Alive Task",
        "Net Client Comm Check Server Silence Task", "Net Client Comm Ping Task",
        "Net Client Comm Find Age Task",
        "Net Client Comm Set Active Player Task",
        "Net Client Comm Get Player List Task",
        "Net Client Comm Create Player Task", "Net Client Comm Join Age Task",
        "Vault Admin Initialization Task", "Multistage Mod Msg",
        "Sound Volume Applicator", "Cutter", "Bullet Msg",
        "Dyna Decal Enable Msg", "Omni Cutoff Applicator", "Armature Update Msg",
        "Avatar Foot Msg", "Net Ownership Msg", "Net Msg Relevance Regions",
        "Particle Flock Msg", "Avatar Behavior Notify Msg", "ATC Channel",
        "Scalar SDL Channel", "Load Avatar Msg", "Avatar Set Type Msg",
        "Net Msg Load Clone", "Net Msg Player Page", "V-Node Init Task",
        "Ripple Shape Msg", "Event Manager",
        "Vault Neighborhood Initialization Task",
        "Net Server Msg Agent Recovery Request",
        "Net Server Msg Frontend Recovery Request",
        "Net Server Msg Backend Recovery Request",
        "Net Server Msg Agent Recovery Data",
        "Net Server Msg Frontend Recovery Data",
        "Net Server Msg Backend Recovery Data", "Sub-World Msg",
        "Matrix Difference App", "Av Brain User", "DX9 Pipeline", "DX Pipeline",
        "Player Msg", "Av Brain Pirahna", "Axis Event Msg", "Crossfade Msg",
        "Subtitle Msg", "SDL Store Msg", "Omni Sq Applicator",
        "Pre Resource Msg", "Dynamic ColorRGBA", "Dynamic Matrix33",
        "Dynamic Matrix44", "Compound Controller", "Net Client Mgr Msg",
        "Age Loader Msg", "Av Brain Avatar", "Python Notify Msg",
        "NC Age Joiner Msg", "Av Task Dumb Seek", "Av Task Smart Seek",
        "Creatable Uuid", "Preloader Msg", "Net Comm Auth Connected Msg",
        "Net Comm Auth Msg", "Net Comm File List Msg",
        "Net Comm File Download Msg", "Net Comm Link To Age Msg",
        "Net Comm Player List Msg", "Net Comm Active Player Msg",
        "Net Comm Create Player Msg", "Net Comm Delete Player Msg",
        "Net Comm Public Age List Msg", "Net Comm Public Age Msg",
        "Net Comm Register Age Msg", "Anim Eval Msg", "Av Brain Flight",
        "Av Brain NPC", "Av Brain Blimp", "Av Brain Flight NPC",
        "Particle Bullet Hit Msg", "Panic Link Msg", "Av Task One-Shot",
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,

        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, "Vault Game Server Initialization Task",
        "Net Client Find Default Age Task", "Vault Age Node",
        "Vault Age Initialization Task", "Set Listener Msg", "Vault System Node",
        "Av Brain Swim", "Net Msg Vault", "Net Server Msg Vault", "Vault Task",
        "Vault Connect Task", "Vault Negotiate Manifest Task",
        "Vault Fetch Nodes Task", "Vault Save Node Task", "Vault Find Node Task",
        "Vault Add Node Ref Task", "Vault Remove Node Ref Task",
        "Vault Send Node Task", "Vault Notify Operation Callback Task",
        "V-Node Mgr Initialization Task", "Vault Player Initialization Task",
        "Net Vault Server Initialization Task", "Common Neighborhoods Init Task",
        "Vault Node Ref", "Vault Node", "Vault Folder Node", "Vault Image Node",
        "Vault Text Note Node", "Vault SDL Node", "Vault Age Link Node",
        "Vault Chronicle Node", "Vault Player Info Node", "Vault Mgr Node",
        "Vault Player Node", "Synch Enable Msg", "Net Vault Server Node",
        "Vault Admin Node", "Vault Game Server Node",
        "Vault Player Info List Node", "Avatar Stealth Mode Msg",
        "Event Callback Intercept Msg", "Dynamic Env Map Msg", "Climb Msg",
        "Iface Fade Avatar Msg", "Av Brain Climb", "Shared Mesh BC Msg",
        "Net Voice List Msg", "Swim Msg", "Morph Delta",
        "Matrix Controller Cache Channel", "Vault Marker Node", "Marker Msg",
        "Pipe Res Make Msg", "Pipe RT Make Msg", "Pipe Geo Make Msg",
        "Av Coop Msg", "Av Brain Coop", "Sim Suppress Msg",
        "Vault Marker List Node", "Av Task Orient", "Age Begin Loading Msg",
        "Set Net Group ID Msg", "Backdoor Msg", "Net Msg Python", "Python Msg",
        "State Data Record", "Net Client Comm Delete Player Task",
        "Net Msg Set Timeout", "Net Msg Active Player Set",
        "Net Client Comm Set Timeout Task", "Net Routable Msg Omnibus",
        "Net Msg Get Public Age List", "Net Msg Public Age List",
        "Net Msg Create Public Age", "Net Msg Public Age Created",
        "Net Server Msg Envelope", "Net Client Comm Get Public Age List Task",
        "Net Client Comm Create Public Age Task", "Net Server Msg Pending Msgs",
        "Net Server Msg Request Pending Msgs", "Db Interface",
        "Db Proxy Interface", "DB Generic SQL DB", "MySql DB",
        "Net Generic Database", "Net Vault Database",
        "Net Server Msg Pls Update Player Reply", "Vault Disconnect Task",
        "Net Client Comm Set Age Public Task",
        "Net Client Comm Register Owned Age",
        "Net Client Comm Unregister Owner Age",
        "Net Client Comm Register Visit Age",
        "Net Client Comm Unregister Visit Age", "Net Msg Remove Public Age",
        "Net Msg Public Age Removed", "Net Client Comm Remove Public Age Task",
        "CCR Message", "Av One-Shot Link Task", "Net Auth Database",
        "Avatar Opacity Callback Msg", "AG Detach Callback Msg",
        "Movie Event Msg", "Movie Msg", "Pipe Tex Make Msg", "Event Log",
        "Db Event Log", "Syslog Event Log", "Capture Render Msg",
        "Age Loaded 2 Msg", "Pseudo Link Effect Msg",
        "Pseudo Link Anim Trigger Msg", "Pseudo Link Anim Callback Msg",
        "Climbing Wall Msg", "Climb Event Msg", "Av Brain Quab",
        "SDL Game Time Elapsed Var", "Link Effects Done Msg", "SDL Var",
        "SDL Struct Var", "SDL Bool Var", "SDL Char Var", "SDL Byte Var",
        "SDL Int Var", "SDL UInt Var", "SDL Float Var", "SDL Double Var",
        "SDL String Var", "SDL Time Var", "SDL Uoid Var", "SDL Vector3 Var",
        "SDL Point3 Var", "SDL Quaternion Var", "SDL Matrix44 Var",
        "SDL RGBA Var", "SDL Age Time Of Day Var", "SDL Age Time Elapsed Var",
        "SDL Meta Double Var", "SDL Fixed Array Struct Var",
        "SDL Fixed Array Bool Var", "SDL Fixed Array Char Var",
        "SDL Fixed Array Byte Var", "SDL Fixed Array Int Var",
        "SDL Fixed Array UInt Var", "SDL Fixed Array Float Var",
        "SDL Fixed Array Double Var", "SDL Fixed Array String Var",
        "SDL Fixed Array Time Var", "SDL Fixed Array Uoid Var",
        "SDL Fixed Array Vector3 Var", "SDL Fixed Array Point3 Var",
        "SDL Fixed Array Quaternion Var", "SDL Fixed Array Matrix44 Var",
        "SDL Fixed Array RGBA Var", "SDL Dyn Array Struct Var",
        "SDL Dyn Array Bool Var", "SDL Dyn Array Char Var",
        "SDL Dyn Array Byte Var", "SDL Dyn Array Int Var",
        "SDL Dyn Array UInt Var", "SDL Dyn Array Float Var",
        "SDL Dyn Array Double Var", "SDL Dyn Array String Var",
        "SDL Dyn Array Time Var", "SDL Dyn Array Uoid Var",
        "SDL Dyn Array Vector3 Var", "SDL Dyn Array Point3 Var",
        "SDL Dyn Array Quaternion Var", "SDL Dyn Array Matrix44 Var",
        "SDL Dyn Array RGBA Var", "SDL Array Var", "SDL Var Change Msg",
        "Av Brain Path", "SDL Buffer Var", "SDL Fixed Array Buffer Var",
        "SDL Dyn Array Buffer Var", "Matrix Borrowed Channel", "Node Region Msg",
        "Event Callback Setup Msg", "Relative Matrix Channel Applicator",
        "Piranha Region Msg", "Game Mgr Msg", "Game Cli Msg", "Game Cli",
        "Gm Tic-Tac-Toe", "Gm Heek", "Gm Marker", "Gm Blue Spiral",
        "Account Update Msg", "AI Msg", "AI Brain Created Msg",
        "Gm Climbing Wall", "AI Arrived At Goal Msg", "Gm Var Sync",
        "Linear Velocity Msg", "Angular Velocity Msg", "Ride Animated Phys Msg",
        "Av Brain Ride Animated Physical", S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID,
        S_INVALID, S_INVALID, S_INVALID, S_INVALID, S_INVALID
    };

    classType &= ~0x3000;
    if ((unsigned short)classType == 0x8000)
        return "(NULL)";
    if (classType >= 0 && classType < TYPESPACE_MAX) {
        if (s_showTypeIDs)
            return QString("[%1] %2").arg(classType, 4, 16, QChar('0')).toUpper()
                                     .arg(s_names[classType]);
        else
            return s_names[classType];
    }
    return S_INVALID;
}

std::vector<short> pqGetValidKOTypes()
{
    // Please keep this sorted alphabetically by friendly class names
    static short s_typeList[] = {
        k2WayWinAudible, kAudible, kAudibleNull, kAudioInterface,
        kCollisionDetector, kCoordinateInterface, kCubicEnvironmap,
        kDetectorModifier, kDirectShadowMaster, kDrawInterface, kDynamicTextMap,
        kFadeOpacityMod, kGUIButtonMod, kGUICheckBoxCtrl, kGUIClickMapCtrl,
        kGUIDialogMod, kGUIDragBarCtrl, kGUIDraggableMod, kGUIDynDisplayCtrl,
        kGUIEditBoxMod, kGUIKnobCtrl, kGUIListBoxMod, kGUIMenuItem,
        kGUIMultiLineEditCtrl, kGUIPopUpMenu, kGUIProgressCtrl,
        kGUIRadioGroupCtrl, kGUISkin, kGUITextBoxMod, kGUIUpDownPairMod,
        kImageLibMod, kInterfaceInfoModifier, kLayer, kLayerAnimation,
        kLayerAVI, kLayerBink, kLayerDepth, kLayerLinkAnimation, kLayerMovie,
        kLayerSDLAnimation, kGMaterial, kMipmap, kMsgForwarder,
        kMultistageBehMod, kOneShotMod, kPanicLinkRegion, kPickingDetector,
        kPointShadowMaster, kPythonFileMod, kSceneNode, kSceneObject,
        kShadowMaster, kSimulationInterface, kSound, kSoundBuffer,
        kSpawnModifier, kSubworldRegionDetector, kWinAudible, kWin32LinkSound,
        kWin32Sound, kWin32StaticSound, kWin32StreamingSound,
    };
    static size_t s_numTypes = sizeof(s_typeList) / sizeof(s_typeList[0]);

    return std::vector<short>(s_typeList, s_typeList + s_numTypes);
}

bool pqCanPreviewType(plCreatable* pCre)
{
    short type = pCre->ClassIndex();
    static short s_typeList[] = {
        kCoordinateInterface, kCubicEnvironmap, kMipmap, kSceneNode,
        kSimulationInterface
    };
    static size_t s_numTypes = sizeof(s_typeList) / sizeof(s_typeList[0]);

    if (type == kSceneObject) {
        plSceneObject* tmp = plSceneObject::Convert(pCre);
        return tmp->getDrawInterface().Exists();
    }

    for (size_t i=0; i<s_numTypes; i++) {
        if (type == s_typeList[i])
            return true;
    }
    return false;
}
