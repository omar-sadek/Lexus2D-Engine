Scene = {
    assets = {
        [0] =
        { type = "texture", id = "solaria-tileset", file = "./assets/tilemaps/solaria.png" },
        { type = "texture", id = "projectile-image",  file = "./assets/images/bullet.png" },
        { type = "texture", id = "character-sprites",  file = "./assets/images/character-sprites.png" },
        { type = "texture", id = "character-ui-sprites",  file = "./assets/images/character-ui.png" },
        { type = "texture", id = "pink-slime",  file = "./assets/images/pink-slime.png" },
        { type = "texture", id = "purple-slime",  file = "./assets/images/purple-slime.png" },
        { type = "font"   , id = "pico8-font-5",    file = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font"   , id = "pico8-font-10",   file = "./assets/fonts/pico8.ttf", font_size = 10 },
        { type = "font"   , id = "charriot-font_s",   file = "./assets/fonts/charriot.ttf", font_size = 10 },
        { type = "font"   , id = "charriot-font_l",   file = "./assets/fonts/charriot.ttf", font_size = 20 }
    },

    tilemap = {
        [0] =
        {
        map_path = "./assets/tilemaps/solaria-background.map",
        texture_asset_id = "solaria-tileset",
        num_rows = 15,
        num_cols = 28,
        tile_size = 16,
        tile_scale = 2,
        walkable = true
        },
        {
        map_path = "./assets/tilemaps/solaria-details.map",
        texture_asset_id = "solaria-tileset",
        num_rows = 15,
        num_cols = 28,
        tile_size = 16,
        tile_scale = 2,
        walkable = true
        },
        {
        map_path = "./assets/tilemaps/solaria-unwalkable.map",
        texture_asset_id = "solaria-tileset",
        num_rows = 15,
        num_cols = 28,
        tile_size = 16,
        tile_scale = 2,
        walkable = false
        }
    }
}