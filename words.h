#pragma once

#include "impostor.h"

// Word counts per category
#define WORDS_PER_CATEGORY 50

// ============================================================================
// ENGLISH WORD LISTS
// ============================================================================

static const char* const words_animals_en[] = {
    "Dog", "Cat", "Elephant", "Lion", "Tiger",
    "Bear", "Wolf", "Fox", "Rabbit", "Horse",
    "Cow", "Pig", "Chicken", "Duck", "Eagle",
    "Shark", "Dolphin", "Whale", "Snake", "Frog",
    "Monkey", "Gorilla", "Zebra", "Giraffe", "Hippo",
    "Crocodile", "Turtle", "Penguin", "Owl", "Parrot",
    "Deer", "Moose", "Kangaroo", "Koala", "Panda",
    "Camel", "Octopus", "Jellyfish", "Crab", "Lobster",
    "Butterfly", "Spider", "Ant", "Bee", "Bat",
    "Squirrel", "Raccoon", "Skunk", "Hedgehog", "Mouse"
};

static const char* const words_food_en[] = {
    "Pizza", "Burger", "Sushi", "Pasta", "Taco",
    "Salad", "Soup", "Steak", "Chicken", "Rice",
    "Bread", "Cheese", "Egg", "Apple", "Banana",
    "Orange", "Cake", "Cookie", "Ice Cream", "Chocolate",
    "Sandwich", "Hotdog", "Pancake", "Waffle", "Donut",
    "Popcorn", "Pretzel", "Nachos", "Burrito", "Quesadilla",
    "Spaghetti", "Lasagna", "Ravioli", "Ramen", "Pho",
    "Shrimp", "Salmon", "Lobster", "Oyster", "Ceviche",
    "Mango", "Pineapple", "Strawberry", "Watermelon", "Grapes",
    "Carrot", "Broccoli", "Tomato", "Avocado", "Potato"
};

static const char* const words_places_en[] = {
    "Beach", "Mountain", "Library", "Hospital", "School",
    "Restaurant", "Park", "Museum", "Airport", "Station",
    "Church", "Castle", "Bridge", "Island", "Desert",
    "Forest", "Lake", "River", "City", "Village",
    "Mall", "Cinema", "Theater", "Stadium", "Arena",
    "Zoo", "Aquarium", "Circus", "Carnival", "Amusement Park",
    "Hotel", "Motel", "Resort", "Cabin", "Tent",
    "Office", "Factory", "Warehouse", "Farm", "Ranch",
    "Bakery", "Pharmacy", "Bank", "Post Office", "Police",
    "Gym", "Pool", "Spa", "Salon", "Barbershop"
};

static const char* const words_objects_en[] = {
    "Chair", "Phone", "Book", "Lamp", "Clock",
    "Table", "Door", "Window", "Mirror", "Bed",
    "Pencil", "Scissors", "Hammer", "Key", "Wallet",
    "Glasses", "Watch", "Camera", "Guitar", "Ball",
    "Laptop", "Tablet", "Keyboard", "Mouse", "Headphones",
    "Television", "Remote", "Speaker", "Microphone", "Radio",
    "Umbrella", "Backpack", "Suitcase", "Purse", "Belt",
    "Pillow", "Blanket", "Towel", "Soap", "Toothbrush",
    "Knife", "Fork", "Spoon", "Plate", "Cup",
    "Candle", "Vase", "Picture", "Rug", "Curtain"
};

static const char* const words_actions_en[] = {
    "Running", "Swimming", "Cooking", "Dancing", "Singing",
    "Reading", "Writing", "Sleeping", "Eating", "Drinking",
    "Jumping", "Walking", "Driving", "Flying", "Climbing",
    "Fishing", "Painting", "Playing", "Working", "Studying",
    "Laughing", "Crying", "Smiling", "Yelling", "Whispering",
    "Pushing", "Pulling", "Throwing", "Catching", "Kicking",
    "Hugging", "Kissing", "Waving", "Clapping", "Snapping",
    "Typing", "Texting", "Calling", "Listening", "Watching",
    "Cleaning", "Washing", "Ironing", "Folding", "Sweeping",
    "Shopping", "Paying", "Selling", "Buying", "Trading"
};

// ============================================================================
// SPANISH WORD LISTS
// ============================================================================

static const char* const words_animals_es[] = {
    "Perro", "Gato", "Elefante", "Leon", "Tigre",
    "Oso", "Lobo", "Zorro", "Conejo", "Caballo",
    "Vaca", "Cerdo", "Pollo", "Pato", "Aguila",
    "Tiburon", "Delfin", "Ballena", "Serpiente", "Rana",
    "Mono", "Gorila", "Cebra", "Jirafa", "Hipopotamo",
    "Cocodrilo", "Tortuga", "Pinguino", "Buho", "Loro",
    "Venado", "Alce", "Canguro", "Koala", "Panda",
    "Camello", "Pulpo", "Medusa", "Cangrejo", "Langosta",
    "Mariposa", "Arana", "Hormiga", "Abeja", "Murcielago",
    "Ardilla", "Mapache", "Zorrillo", "Erizo", "Raton"
};

static const char* const words_food_es[] = {
    "Pizza", "Hamburguesa", "Sushi", "Pasta", "Taco",
    "Ensalada", "Sopa", "Bistec", "Pollo", "Arroz",
    "Pan", "Queso", "Huevo", "Manzana", "Platano",
    "Naranja", "Pastel", "Galleta", "Helado", "Chocolate",
    "Sandwich", "Hotdog", "Panqueque", "Waffle", "Dona",
    "Palomitas", "Pretzel", "Nachos", "Burrito", "Quesadilla",
    "Espagueti", "Lasana", "Ravioli", "Ramen", "Pho",
    "Camaron", "Salmon", "Langosta", "Ostra", "Ceviche",
    "Mango", "Pina", "Fresa", "Sandia", "Uvas",
    "Zanahoria", "Brocoli", "Tomate", "Aguacate", "Papa"
};

static const char* const words_places_es[] = {
    "Playa", "Montana", "Biblioteca", "Hospital", "Escuela",
    "Restaurante", "Parque", "Museo", "Aeropuerto", "Estacion",
    "Iglesia", "Castillo", "Puente", "Isla", "Desierto",
    "Bosque", "Lago", "Rio", "Ciudad", "Pueblo",
    "Centro Comercial", "Cine", "Teatro", "Estadio", "Arena",
    "Zoologico", "Acuario", "Circo", "Carnaval", "Parque",
    "Hotel", "Motel", "Resort", "Cabana", "Tienda",
    "Oficina", "Fabrica", "Almacen", "Granja", "Rancho",
    "Panaderia", "Farmacia", "Banco", "Correo", "Policia",
    "Gimnasio", "Piscina", "Spa", "Salon", "Barberia"
};

static const char* const words_objects_es[] = {
    "Silla", "Telefono", "Libro", "Lampara", "Reloj",
    "Mesa", "Puerta", "Ventana", "Espejo", "Cama",
    "Lapiz", "Tijeras", "Martillo", "Llave", "Cartera",
    "Gafas", "Reloj", "Camara", "Guitarra", "Pelota",
    "Laptop", "Tableta", "Teclado", "Raton", "Audifonos",
    "Television", "Control", "Bocina", "Microfono", "Radio",
    "Paraguas", "Mochila", "Maleta", "Bolsa", "Cinturon",
    "Almohada", "Cobija", "Toalla", "Jabon", "Cepillo",
    "Cuchillo", "Tenedor", "Cuchara", "Plato", "Taza",
    "Vela", "Florero", "Cuadro", "Alfombra", "Cortina"
};

static const char* const words_actions_es[] = {
    "Correr", "Nadar", "Cocinar", "Bailar", "Cantar",
    "Leer", "Escribir", "Dormir", "Comer", "Beber",
    "Saltar", "Caminar", "Manejar", "Volar", "Escalar",
    "Pescar", "Pintar", "Jugar", "Trabajar", "Estudiar",
    "Reir", "Llorar", "Sonreir", "Gritar", "Susurrar",
    "Empujar", "Jalar", "Lanzar", "Atrapar", "Patear",
    "Abrazar", "Besar", "Saludar", "Aplaudir", "Tronar",
    "Teclear", "Textear", "Llamar", "Escuchar", "Mirar",
    "Limpiar", "Lavar", "Planchar", "Doblar", "Barrer",
    "Comprar", "Pagar", "Vender", "Comprar", "Cambiar"
};

// ============================================================================
// CATEGORY NAMES
// ============================================================================

static const char* const category_names_en[] = {
    "Animals",
    "Food",
    "Places",
    "Objects",
    "Actions"
};

static const char* const category_names_es[] = {
    "Animales",
    "Comida",
    "Lugares",
    "Objetos",
    "Acciones"
};

// ============================================================================
// WORD LIST ARRAYS (indexed by category)
// ============================================================================

static const char* const* const words_en[] = {
    words_animals_en,
    words_food_en,
    words_places_en,
    words_objects_en,
    words_actions_en
};

static const char* const* const words_es[] = {
    words_animals_es,
    words_food_es,
    words_places_es,
    words_objects_es,
    words_actions_es
};
