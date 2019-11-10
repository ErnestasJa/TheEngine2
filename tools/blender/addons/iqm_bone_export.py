# This script is licensed as public domain.

bl_info = {
    "name": "Export mesh bone data",
    "author": "Me",
    "version": (2019, 10, 27),
    "blender": (2, 80, 0),
    "location": "File > Export > MBD Export",
    "description": "Export to the MBD format (.mbd)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import os, struct, math
import mathutils
import bpy
import bpy_extras.io_utils

MBD_HEADER = struct.Struct('<4s4i')
MBD_BONE = struct.Struct('<3i6f')

class Bone:
    def __init__(self, bone: bpy.types.Bone, index, parent):
        self.name = bone.name
        self.head_pos = [bone.head_local.x, bone.head_local.y, bone.head_local.z]
        self.tail_pos = [bone.tail_local.x, bone.tail_local.y, bone.tail_local.z]
        self.index = index
        self.parent = parent.index if parent is not None else -1
        print(bone)
         
    def data(self, mbdFile):
        data = [mbdFile.addText(self.name), self.index, self.parent]
        data.extend(self.head_pos)
        data.extend(self.tail_pos)
        return data
        
    def __str__(self):
        return ("Bone(name = '" + self.name 
                + "', head = " + str(self.head_pos)
                + ", tail = " + str(self.tail_pos) 
                + ", index = " + str(self.index) 
                + ", parent = " + str(self.parent) 
                + " )")
    
    def __repr__(self):
        return self.__str__()
    
    def add(self, bdmFile):
        return

class MBDFile:
    def __init__(self):
        self.textoffsets = {}
        self.textdata = b''
        self.bones = []
        self.text_num = 0
        self.text_offset = 0
        self.bone_num = 0
        self.bone_offset = 0
        
        self.current_offset = 0
        
    def makeOffset(self, dataCount, dataSize):
        if dataCount > 0:
            offset = self.current_offset
            self.current_offset += dataCount * dataSize
            return offset
        
        return 0
        
    def addText(self, str):
        if not self.textdata:
            self.textdata += b'\x00'
            self.textoffsets[''] = 0
        try:
            return self.textoffsets[str]
        except:
            offset = len(self.textdata)
            self.textoffsets[str] = offset
            self.textdata += bytes(str, encoding="utf8") + b'\x00'
            return offset
        
    def addBones(self, bones):
        for bone in bones:
            self.bones.append(bone.data(self))
        return
    
    def calcOffsets(self):
        if self.textdata:
            while len(self.textdata) % 4:
                self.textdata += b'\x00'
        
        self.makeOffset(1, MBD_HEADER.size)
        
        self.text_num = len(self.textdata)
        self.text_offset = self.makeOffset(self.text_num, 1)
        
        self.bone_num = len(self.bones)
        self.bone_offset = self.makeOffset(self.bone_num, MBD_BONE.size)
    
    def write(self, file):
        
        self.writeHeader(file)
        
        self.writeText(file)
        
        self.writeBones(file)
            
        return 
    
    def writeText(self, file):
        file.write(self.textdata)
    
    def writeBones(self, file):
        for bone in self.bones:
            file.write(MBD_BONE.pack(*bone))
    
    def writeHeader(self, file):
        self.calcOffsets()
        
        file.write(MBD_HEADER.pack('MBD'.encode('ascii'), self.text_offset, self.text_num, self.bone_offset, self.bone_num))
 
 
def collectBones(armature):
    data = armature.data
    worldmatrix = armature.matrix_world
    
    bones = []
    worklist = [ bone for bone in data.bones.values() if not bone.parent ]
    
    for index, bone in enumerate(worklist):
        
        bones.append(Bone(bone, index, bone.parent and next(i for i in bones if i.name == bone.parent.name)))
        
        for child in bone.children:
            if child not in worklist:
                worklist.append(child)
                
    print('Collected bones:' + str(bones))
    return bones
 
def exportMBD(context, filename):
    armature = findArmature(context)
    if not armature:
        print('No armature selected')
        return
    
    file = open(filename, 'wb')
    
    mbdFile = MBDFile()
    
    mbdFile.addBones(collectBones(armature))
    
    mbdFile.write(file)
    
    file.close()
    
    return



def findArmature(context):
    armature = None
    for obj in context.selected_objects:
        if obj.type == 'ARMATURE':
            armature = obj
            break
    if not armature:
        for obj in context.selected_objects:
            if obj.type == 'MESH':
                armature = obj.find_armature()
                if armature:
                    break
    return armature


class ExportMBD(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    '''Export a MBD file'''
    bl_idname = "export.mbd"
    bl_label = 'Export MBD'
    filename_ext = ".mbd"
    
    animspec = bpy.props.StringProperty(name="Animations", description="Animations to export", maxlen=1024, default="")
    usemesh = bpy.props.BoolProperty(name="Meshes", description="Generate meshes", default=True)
    usemods = bpy.props.BoolProperty(name="Modifiers", description="Apply modifiers", default=True)
    useskel = bpy.props.BoolProperty(name="Skeleton", description="Generate skeleton", default=True)
    usebbox = bpy.props.BoolProperty(name="Bounding boxes", description="Generate bounding boxes", default=True)
    usecol = bpy.props.BoolProperty(name="Vertex colors", description="Export vertex colors", default=False)
    usescale = bpy.props.FloatProperty(name="Scale", description="Scale of exported model", default=1.0, min=0.0, step=50, precision=2)
    derigify = bpy.props.BoolProperty(name="De-rigify", description="Export only deformation bones from rigify", default=False)
    boneorder = bpy.props.StringProperty(name="Bone order", description="Override ordering of bones", subtype="FILE_NAME", default="")

    def execute(self, context):
        print("Export started")
        exportMBD(context, self.properties.filepath)
        print("Export finished")
        return {'FINISHED'}

    def check(self, context):
        filepath = bpy.path.ensure_ext(self.filepath, '.mbd')
        if filepath != self.filepath:
            self.filepath = filepath
            return True
        return False
 


def menu_func(self, context):
    self.layout.operator(ExportMBD.bl_idname, text="MBD (.mbd)")


def register():
    bpy.utils.register_class(ExportMBD)
    bpy.types.TOPBAR_MT_file_export.append(menu_func)
    print("registered MBD exporter")

def unregister():
    bpy.utils.unregister_class(ExportMBD)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func)
    print("unregistered MBD exporter")

if __name__ == "__main__":
    register()

    