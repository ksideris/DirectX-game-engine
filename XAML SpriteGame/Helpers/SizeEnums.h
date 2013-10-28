#pragma once

enum class PositionUnits
{
    DIPs,         // Interpret position as device-independent pixel values.
    Pixels,       // Interpret position as pixel values.
    Normalized,   // Interpret position as a fraction of the render target dimensions.
    UniformWidth, // Interpret position as a fraction of the render target width.
    UniformHeight // Interpret position as a fraction of the render target height.
};

enum class SizeUnits
{
    DIPs,      // Interpret size as device-independent pixel values.
    Pixels,    // Interpret size as pixel values.
    Normalized // Interpret size as a multiplier of the pixel size of the sprite.
};

enum class BlendMode
{
    Alpha,   // Use alpha blending (out = old * (1 - new.a) + new * new.a).
    Additive // Use additive blending (out = old + new * new.a).
};

enum class RenderTechnique
{
    Replication,
    Instancing,
    GeometryShader
};