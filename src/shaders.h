#pragma once
#include <glm/glm.hpp>
#include "uniforms.h"
#include "fragment.h"
#include "color.h"
#include <random>
#include "print.h"
#include "FastNoiseLite.h"



// Para cambiar de planeta, cambia el valor de selectedPlanet a un número del 1 al 6
// 1: Gaseoso de helado con tormentas azules
// 2: Rocoso (Mar con islas con grama y con lagos)
// 3: Estrella (Sol)
// 4: Random 1 (Pelota de tripa de coche)
// 5: Random 2 (Agua con islas de panqueque y huevo estrellado)
// 6: Random 3 (Planeta congelado con volcanes rojos)
int selectedPlanet = 2;

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.position,
        // vertex.normal, // non transformed normal
    };
}


// Planeta 1: Gaseoso
Fragment fragmentShader(const Fragment& fragment) {

if (selectedPlanet == 1) {
        Color color;

        // Define los colores base y los colores de las capas
        glm::vec3 baseColor = glm::vec3(0.88f, 0.66f, 0.94f);
        glm::vec3 secondColor = glm::vec3(0.68f, 0.66f, 0.94f);
        glm::vec3 thirdColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 fourthColor = glm::vec3(0.0f, 0.0f, 0.2f); // Color azul oscuro

        glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        // Ajusta estos valores para controlar la apariencia de las capas
        float baseLayerThreshold = 0.6f;
        float secondLayerThreshold = 0.8f;

        float ox = 1200.0f;
        float oy = 3000.0f;
        float zoom = 200.0f;

        float noiseValue = noiseGenerator.GetNoise((uv.x + ox) * zoom, (uv.y + oy) * zoom);

        glm::vec3 tmpColor = baseColor;

        if (noiseValue < baseLayerThreshold) {
            tmpColor = baseColor;
        } else if (noiseValue < secondLayerThreshold) {
            // Interpolamos entre el color base y el segundo color
            float t = (noiseValue - baseLayerThreshold) / (secondLayerThreshold - baseLayerThreshold);
            tmpColor = glm::mix(baseColor, secondColor, t);
        } else {
            // Cambiamos el color de la tormenta al cuarto color (azul oscuro)
            tmpColor = fourthColor;
        }

        // Agregar la mancha de tormenta del color secondColor
        glm::vec2 stormCenter = glm::vec2(0.45f, 0.55f); // Ajusta el centro de la tormenta
        float stormRadius = 0.4f; // Ajusta el radio de la tormenta
        float distanceToStorm = glm::distance(uv, stormCenter);

        if (distanceToStorm < stormRadius) {
            // Ajusta la opacidad e intensidad de la tormenta
            float opacity = 0.7f; // Ajusta la opacidad deseada
            tmpColor = glm::mix(tmpColor, secondColor, opacity);
        }

        // Puedes ajustar estos valores según tus preferencias para la apariencia de las capas
        float cloudLayerThreshold = 0.5f;
        float cloudDensity = 0.5f;

        float oxc = 5500.0f;
        float oyc = 6900.0f;
        float zoomc = 300.0f;

        float noiseValueC = noiseGenerator.GetNoise((uv.x + oxc) * zoomc, (uv.y + oyc) * zoomc);

        if (noiseValueC < cloudLayerThreshold) {
            // Agrega ruido de nubes utilizando el tercer color
            tmpColor = glm::mix(tmpColor, thirdColor, cloudDensity);
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;
        processedFragment.color = color * fragment.intensity;

        return processedFragment;
    }




// Planeta 2: Rocoso (Mar con islas con grama y con lagos)

if (selectedPlanet == 2) {
        Color color;

        // Color base #9cdbff
        glm::vec3 baseColor = glm::vec3(0.61f, 0.86f, 1.0f);

        // Configuración de ruido para los patrones
        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        float noiseScale = 1000.0f; // Ajusta la escala del ruido para los patrones
        float noiseValue = noiseGenerator.GetNoise(fragment.original.x * noiseScale, fragment.original.y * noiseScale, fragment.original.z * noiseScale);

        // Calcula el color final mezclando el color base con los patrones
        glm::vec3 tmpColor = baseColor;

        // Define umbrales para las capas de patrones
        float patternThreshold1 = 0.2f; // Umbral para el primer patrón
        float patternThreshold2 = 0.6f; // Umbral para el segundo patrón
        float patternThreshold3 = 0.9f; // Umbral para el tercer patrón

        // Asigna colores a las capas de patrones #00f2ff, #a8ff96, #ffffff
        glm::vec3 patternColor1 = glm::vec3(0.0f, 0.95f, 1.0f); // #00f2ff
        glm::vec3 patternColor2 = glm::vec3(0.66f, 1.0f, 0.59f); // #a8ff96
        glm::vec3 patternColor3 = glm::vec3(1.0f, 1.0f, 1.0f); // #ffffff

        // Aplica los colores de las capas de patrones según el valor de ruido
        if (noiseValue < patternThreshold1) {
            tmpColor = patternColor1;
        } else if (noiseValue < patternThreshold2) {
            tmpColor = patternColor2;
        } else if (noiseValue < patternThreshold3) {
            tmpColor = patternColor3;
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;

        processedFragment.color = color * fragment.intensity;

        return processedFragment;
    }




// Planeta 3: Estrella

if (selectedPlanet == 3) {
        Color color;

        // Color base amarillo para el sol
        glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 0.0f); // Amarillo brillante

        // Configuración de ruido para los patrones
        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        float noiseScale = 5000.0f; // Ajusta la escala del ruido para los patrones
        float noiseValue = noiseGenerator.GetNoise(fragment.original.x * noiseScale, fragment.original.y * noiseScale, fragment.original.z * noiseScale);

        // Calcula el color final mezclando el color base con los patrones
        glm::vec3 tmpColor = baseColor;

        // Define umbrales para las capas de patrones
        float patternThreshold1 = 0.1f; // Umbral para el primer patrón
        float patternThreshold2 = 0.5f; // Umbral para el segundo patrón
        float patternThreshold3 = 0.9f; // Umbral para el tercer patrón

        // Asigna colores a las capas de patrones #fffcc2, naranja y rojo
        // glm::vec3 patternColor1 = glm::vec3(1.0f, 0.99f, 0.76f); // #fffcc2
        glm::vec3 patternColor1 = glm::vec3(0.0f, 0.0f, 0.0f); // Negro
        glm::vec3 patternColor2 = glm::vec3(1.0f, 0.5f, 0.0f); // Naranja
        glm::vec3 patternColor3 = glm::vec3(1.0f, 0.0f, 0.0f); // Red

        // Aplica los colores de las capas de patrones según el valor de ruido
        if (noiseValue < patternThreshold1) {
            tmpColor = patternColor1;
        } else if (noiseValue < patternThreshold2) {
            tmpColor = patternColor2;
        } else if (noiseValue < patternThreshold3) {
            tmpColor = patternColor3;
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;
        processedFragment.color = color * fragment.intensity;

        return processedFragment;
    }






// Planeta 4: Random 1 (Pelota de tripa de coche)

if (selectedPlanet == 4) {
        Color color;

        // Define los cuatro colores de capa
        glm::vec3 layer1Color = glm::vec3(1.0f, 0.1f, 0.1f); // Rojo
        glm::vec3 layer2Color = glm::vec3(1.0f, 1.0f, 1.0f); // Blanco
        glm::vec3 layer3Color = glm::vec3(0.0f, 0.2f, 1.0f); // #0091ff
        glm::vec3 holeColor = glm::vec3(0.0f); // Color negro para las divisiones oscuros

        // Configuración de ruido para los hoyos oscuros
        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        float noiseScale = 30.0f; // Ajusta la escala del ruido para el tamaño de los hoyos
        float noiseValue = noiseGenerator.GetNoise(fragment.original.x * noiseScale, fragment.original.y * noiseScale);

        // Convierte fragment.original a glm::vec2
        glm::vec2 fragmentPosition = glm::vec2(fragment.original);

        // Calcula el color final mezclando los colores de capa con los hoyos oscuros
        glm::vec3 tmpColor;

        // Calcula la distancia desde el centro para crear hoyos circulares
        glm::vec2 center = glm::vec2(0.5f, 0.5f); 
        float distanceToCenter = glm::length(fragmentPosition - center);

        // Define el radio de los hoyos circulares
        float holeRadius = 0.1f; // Ajusta el tamaño de los hoyos

        if (distanceToCenter < holeRadius) {
            tmpColor = holeColor; // Color negro para los hoyos oscuros
        } else {
            // Interpola entre los colores de capa basado en el valor de ruido
            float t = (noiseValue + 1.0f) / 2.0f; // Ajusta el rango de [-1, 1] a [0, 1]
            if (t < 0.25f) {
                tmpColor = glm::mix(layer1Color, holeColor, t * 4.0f);
            } else if (t < 0.5f) {
                tmpColor = glm::mix(layer2Color, holeColor, (t - 0.25f) * 4.0f);
            } else if (t < 0.75f) {
                tmpColor = glm::mix(layer3Color, holeColor, (t - 0.5f) * 4.0f);
            } else {
                tmpColor = holeColor;
            }
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;
        processedFragment.color = color;

        return processedFragment;
    }




// Planeta 5: Random 2 (Agua con islas de panqueque y huevo estrellado)

if (selectedPlanet == 5) {
        Color color;

        // Define los colores base y los colores de las capas
        glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 0.0f);      // Amarillo brillante
        glm::vec3 holeColor = glm::vec3(0.0f, 0.0f, 0.2f);       // Azul oscuro para los hoyos
        glm::vec3 spotColor = glm::vec3(0.8f, 0.66f, 0.2f);       // Color marrón para las manchas
        glm::vec3 shineColor = glm::vec3(1.0f, 1.0f, 1.0f);       // Color blanco para el brillo

        // glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

        glm::vec3 uv = glm::vec3(fragment.original.x,  fragment.original.y, fragment.original.z);



        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        // Ajusta estos valores para controlar la apariencia de las capas
        float holeThreshold = 0.3f;     // Umbral para los hoyos
        float spotThreshold = 0.6f;     // Umbral para las manchas
        float shineThreshold = 0.9f;    // Umbral para el brillo

        float ox = 1200.0f;
        float oy = 3000.0f;
        float zoom = 200.0f;

        float noiseValue = noiseGenerator.GetNoise((uv.x + ox) * zoom, (uv.y + oy) * zoom, (uv.z + oy) * zoom);

        glm::vec3 tmpColor = baseColor;

        if (noiseValue < holeThreshold) {
            tmpColor = holeColor; // Hoyos oscuros
        } else if (noiseValue < spotThreshold) {
            tmpColor = spotColor; // Manchas marrones
        } else if (noiseValue < shineThreshold) {
            tmpColor = shineColor; // Brillo blanco
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;
        processedFragment.color = color * fragment.intensity;

        return processedFragment;
    }



// Planeta 6: Random 3 Lava con manchas

if (selectedPlanet == 6) {
    // Declaración de variables globales para controlar el color de la lava
    float lavaColorHue = 0.0f; // Variable para controlar el tono del color
    float lavaColorSpeed = 0.001f; // Velocidad de cambio de color de la lava (ajustado para un cambio más suave)

        Color color;

        // Define los colores base y los colores de las capas
        // Color de lava celeste neon #00ffee
        glm::vec3 neonLavaColor = glm::vec3(0.0f, 1.0f, 0.933f);
        glm::vec3 purpleColor = glm::vec3(0.5f, 0.0f, 0.5f); // Color morado para las placas de tierra
        glm::vec3 crackColor = glm::vec3(1.0f, 0.0f, 0.0f); // Color de las grietas (rojo)

        glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        // Ajusta estos valores para controlar la apariencia de las divisiones
        float divisionThreshold = 0.6f;
        float plateBorderWidth = 0.05f; // Ancho de los bordes de las placas
        float crackThreshold = 0.7f; // Umbral para las grietas (ajustado para que estén arriba)

        float ox = 1200.0f;
        float oy = 3000.0f;
        float zoom = 200.0f;

        float noiseValue = noiseGenerator.GetNoise((uv.x + ox) * zoom, (uv.y + oy) * zoom);

        // Calcula el color de lava en movimiento utilizando el valor de hue cambiante
        glm::vec3 tmpColor;

        if (noiseValue < divisionThreshold) {
            // Verifica si el fragmento está en un borde de placa
            float borderNoise = noiseGenerator.GetNoise((uv.x + ox) * zoom, (uv.y + oy) * zoom + 1000.0f);
            if (borderNoise < plateBorderWidth) {
                tmpColor = purpleColor; // Usa el color morado en los bordes de las placas
            } else {
                tmpColor = neonLavaColor; // Usa el color de lava celeste neon en el centro de las placas
            }
        } else {
            tmpColor = purpleColor; // Usa el color morado para el resto del planeta
        }

        // Agrega las grietas rojas encima de las placas
        if (noiseValue < crackThreshold) {
            tmpColor = glm::mix(tmpColor, crackColor, noiseValue / crackThreshold);
        }

        color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

        Fragment processedFragment = fragment;
        processedFragment.color = color * fragment.intensity;

        // Actualiza el tono del color de la lava para el siguiente frame de manera continua
        lavaColorHue += lavaColorSpeed;

        return processedFragment;
    }
}