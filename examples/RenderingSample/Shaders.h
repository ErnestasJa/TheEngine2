#ifndef SHADERS_H
#define SHADERS_H

namespace shader
{
struct ShaderSource {
    core::String Vertex;
    core::String Fragment;
    core::String Geometry;
};

static const ShaderSource NormalMapShader = {
    // clang-format off
R"V0G0N(
#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 vertexUV;
layout(location = 1) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    
    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    
    UV = vertexUV;

    // model to camera = ModelView
    vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
    vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
    vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
    
    mat3 TBN = transpose(mat3(
        vertexTangent_cameraspace,
        vertexBitangent_cameraspace,
        vertexNormal_cameraspace    
    ));

    LightDirection_tangentspace = TBN * LightDirection_cameraspace;
    EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
}

)V0G0N",
 R"V0G0N(
#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

out vec3 color;

layout(binding=0) uniform sampler2D DiffuseTextureSampler;
layout(binding=1) uniform sampler2D NormalTextureSampler;
uniform vec3 LightPosition_worldspace;

void main(){
    vec3 LightColor = vec3(1,1,1);
    float LightPower = 120.0;
    
    // Material properties
    vec3 texColor = texture( DiffuseTextureSampler, UV ).rgb;
    vec3 MaterialDiffuseColor = vec3(0.2,0,0.6) * length(texColor);
    vec3 MaterialAmbientColor = vec3(0,0,0);
    vec3 MaterialSpecularColor = vec3(0.2,0.2,0.2);

    vec3 TextureNormal_tangentspace = normalize(texture( NormalTextureSampler, vec2(UV.x,UV.y) ).rgb*2.0 - 1.0);
    
    float distance = length( LightPosition_worldspace - Position_worldspace );

    vec3 n = TextureNormal_tangentspace;
    vec3 l = normalize(LightDirection_tangentspace);
    float cosTheta = clamp( dot( n,l ), 0,1 );
    vec3 E = normalize(EyeDirection_tangentspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    
    color = 
        MaterialAmbientColor
        +MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance)
        +MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance)
        ;

}
)V0G0N",
""
    // clang-format on
};

static const ShaderSource CellShader{
    R"V0G0N(
        #version 330
        layout(location = 2) in vec3 normal;
        layout(location = 1) in vec3 pos;

        uniform mat4 MVP;
        uniform mat3 MV3x3;

        smooth out vec3 vpos;
        smooth out vec3 vnormal;

        void main()
        {
            vnormal = MV3x3 * normal;
            gl_Position = MVP * vec4(pos,1);
            vpos = gl_Position.xyz;
        }
    )V0G0N",
    R"V0G0N(
        #version 330
        uniform vec3 ldir;
        uniform mat3 MV3x3;

        smooth in vec3 vpos;
        smooth in vec3 vnormal;
        out vec3 color;

        void main()
        {
            float lpower = 120.0;
            float cos_nl = dot(MV3x3*ldir, normalize(vnormal));
            float intensity = cos_nl;

            if (intensity > 0.95)
                color = vec3(1.0,0.5,0.5);
            else if (intensity > 0.5)
                color = vec3(0.6,0.3,0.3);
            else if (intensity > 0.25)
                color = vec3(0.4,0.2,0.2);
            else
                color = vec3(0.2,0.1,0.1);

            color = color;

        }
    )V0G0N"};
}

#endif