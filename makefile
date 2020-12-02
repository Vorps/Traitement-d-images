CC = g++
CFLAGS =

sources=Vector.cpp Matrix.cpp Image.cpp ImageMeasure.cpp ImageTreatment.cpp ImageFactory.cpp ImageBmp.cpp ImageAnalyse.cpp KMean.cpp EM.cpp Otsu.cpp
headers=ImageTreatment.h ImageFile.h ImageFactory.h Image.h ImageBmp.h ImageAnalyse.h ImageMeasure.h Vector.h Matrix.h KMean.h EM.h Segmentation.h Otsu.h
objectsFile=$(addprefix $(buildFolder),$(sources:.cpp=.o))
buildFolder = ./build/

Main : $(sources) $(objectsFile) Main.cpp
	$(CC) $(CFLAGS) -o $@ $(objectsFile) Main.cpp
	./Main

./build/ImageTreatment.o : ImageTreatment.cpp ImageTreatment.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/ImageFactory.o : ImageFactory.cpp ImageFactory.h 
	$(CC) $(CFLAGS) -o $@ -c  $< -nostartfiles

./build/Image.o : Image.cpp Image.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/ImageBmp.o : ImageBmp.cpp ImageBmp.h ImageFile.h
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/ImageAnalyse.o : ImageAnalyse.cpp ImageAnalyse.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/ImageMeasure.o : ImageMeasure.cpp ImageMeasure.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/Vector.o : Vector.cpp Vector.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/Matrix.o : Matrix.cpp Matrix.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/EM.o : EM.cpp EM.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/KMean.o : KMean.cpp KMean.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

./build/Otsu.o : Otsu.cpp Otsu.h 
	$(CC) $(CFLAGS) -o $@ -c $< -nostartfiles

clean:
	rm ./build/*
