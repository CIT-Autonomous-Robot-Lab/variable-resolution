from PIL import Image

img = Image.open('map_tsudanuma_mcl_orign.pgm')

width, height = img.size
#img_resize = img.resize((1472, 1000))
img_resize_lanczos = img.resize((width, height), Image.LANCZOS)
img_resize_lanczos = img.resize((int(img.width / 3), int(img.height / 3)))
#img_resize_lanczos = img.resize((int(img.width * 7), int(img.height * 7)))
img_resize_lanczos.save('map_tsudanuma_mcl_6.pgm')