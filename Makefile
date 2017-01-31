CC = g++
CCFLAGS = -g -Wall

rd_view: libcs680.a rd_direct.o rd_drawLine.o rd_drawCircle.o rd_drawObject.o rd_floodFill.o rd_drawingPipelines.o rd_graphicsPipeline.o rd_lighting.o pnm_display.o
	$(CC) -o rd_view $(CCFLAGS) libcs680.a rd_direct.o rd_drawLine.o rd_drawCircle.o rd_drawObject.o rd_floodFill.o rd_drawingPipelines.o rd_graphicsPipeline.o rd_lighting.o pnm_display.o -lm -lX11

# Add whatever additional files you want with a rule here, and also
# in the final linking rule above.

rd_direct.o: rd_direct.cc rd_direct.h rd_display.h rd_enginebase.h rd_error.h rd_dataStructures.h rd_drawLine.h rd_drawCircle.h rd_drawObject.h rd_floodFill.h rd_drawingPipelines.h rd_graphicsPipeline.h rd_lighting.h
	$(CC) $(CCFLAGS) -c rd_direct.cc

pnm_display.o: pnm_display.cc pnm_display.h rd_error.h
	$(CC) $(CCFLAGS) -c pnm_display.cc

rd_drawLine.o: rd_drawLine.cc rd_drawLine.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_drawLine.cc

rd_drawCircle.o: rd_drawCircle.cc rd_drawCircle.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_drawCircle.cc
	
rd_drawObject.o: rd_drawObject.cc rd_drawObject.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_drawObject.cc

rd_floodFill.o: rd_floodFill.cc rd_floodFill.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_floodFill.cc

rd_drawingPipelines.o: rd_drawingPipelines.cc rd_drawingPipelines.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_drawingPipelines.cc

rd_graphicsPipeline.o: rd_graphicsPipeline.cc rd_graphicsPipeline.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_graphicsPipeline.cc

rd_lighting.o: rd_lighting.cc rd_lighting.h rd_direct.h
	$(CC) $(CCFLAGS) -c rd_lighting.cc

clean:
	-rm *.o rd_view
