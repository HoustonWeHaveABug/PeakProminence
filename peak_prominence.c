#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
	unsigned long y;
	unsigned long x;
	int elevation;
	int visited;
}
cell_t;

typedef struct edge_s edge_t;

struct edge_s {
	edge_t *from;
	cell_t *to;
};

int read_cell(cell_t *, unsigned long, unsigned long);
void set_cell(cell_t *, unsigned long, unsigned long, int);
void peak_prominence(cell_t *);
void queue_push_neighbours(edge_t *, int);
void queue_push(edge_t *, cell_t *, int);
void set_edge(edge_t *, edge_t *, cell_t *);
void reset_queue(void);
void set_key_col_ancestor(cell_t *, edge_t *);
void print_cell(const char *, cell_t *);

unsigned long height, width, queue_size;
cell_t *key_col, *ancestor;
edge_t *queue;

int main(void) {
	unsigned long map_size, y, peaks_n;
	cell_t *map, *map_min, *peak, *cell;
	if (scanf("%lu%lu", &height, &width) != 2 || height < 1UL || width < 1UL) {
		fprintf(stderr, "Invalid map size\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	if (height > ULONG_MAX/width) {
		fprintf(stderr, "Map is too large\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	map_size = height*width;
	map = malloc(sizeof(cell_t)*map_size);
	if (!map) {
		fprintf(stderr, "Could not allocate memory for map\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	for (y = 0UL; y < height; y++) {
		unsigned long x;
		for (x = 0UL; x < width; x++) {
			if (!read_cell(map+y*width+x, y, x)) {
				free(map);
				return EXIT_FAILURE;
			}
		}
	}
	queue = malloc(sizeof(edge_t)*map_size);
	if (!queue) {
		fprintf(stderr, "Could not allocate memory for queue\n");
		fflush(stderr);
		free(map);
		return EXIT_FAILURE;
	}
	peaks_n = 0UL;
	map_min = map;
	for (cell = map+1UL; cell < map+map_size; cell++) {
		if (cell->elevation < map_min->elevation) {
			map_min = cell;
		}
	}
	for (peak = map; peak < map+map_size; peak++) {
		key_col = map_min;
		if (peak != key_col && peak->elevation == key_col->elevation) {
			key_col = peak;
		}
		ancestor = NULL;
		peak_prominence(peak);
		if (peak != key_col) {
			peaks_n++;
			print_cell("Peak", peak);
			printf("Prominence: %u\n", (unsigned)peak->elevation-(unsigned)key_col->elevation);
			print_cell("Key Col", key_col);
			if (ancestor) {
				print_cell("Ancestor", ancestor);
			}
			else {
				puts("Ancestor: none");
			}
			puts("----");
			fflush(stdout);
		}
	}
	printf("Number of peaks: %lu\n", peaks_n);
	fflush(stdout);
	free(queue);
	free(map);
	return EXIT_SUCCESS;
}

int read_cell(cell_t *cell, unsigned long y, unsigned long x) {
	int elevation;
	if (scanf("%d", &elevation) != 1) {
		fprintf(stderr, "Invalid elevation\n");
		fflush(stderr);
		return 0;
	}
	set_cell(cell, y, x, elevation);
	return 1;
}

void set_cell(cell_t *cell, unsigned long y, unsigned long x, int elevation) {
	cell->y = y;
	cell->x = x;
	cell->elevation = elevation;
	cell->visited = 0;
}

void peak_prominence(cell_t *peak) {
	int hi = peak->elevation, lo = key_col->elevation;
	unsigned long i;
	do {
		int mid = hi/2+lo/2+(hi%2+lo%2)/2;
		queue_size = 0UL;
		queue_push(NULL, peak, mid);
		for (i = 0UL; i < queue_size && queue[i].to->elevation <= peak->elevation; i++) {
			queue_push_neighbours(queue+i, mid);
		}
		if (i < queue_size) {
			set_key_col_ancestor(peak, queue+i);
			if (mid == INT_MAX) {
				break;
			}
			lo = mid+1;
		}
		else {
			if (mid == INT_MIN) {
				break;
			}
			hi = mid-1;
		}
		if (hi >= lo) {
			reset_queue();
		}
	}
	while (hi >= lo);
	if (peak != key_col && ancestor) {
		if (i < queue_size) {
			i++;
		}
		else {
			reset_queue();
			queue_size = 0UL;
			queue_push(NULL, peak, key_col->elevation);
			i = 0UL;
		}
		for (; i < queue_size; i++) {
			queue_push_neighbours(queue+i, key_col->elevation);
			if (queue[i].to->elevation > ancestor->elevation) {
				set_key_col_ancestor(peak, queue+i);
			}
		}			
	}
	reset_queue();
}

void queue_push_neighbours(edge_t *edge, int elevation_low) {
	if (edge->to->x > 0UL) {
		queue_push(edge, edge->to-1UL, elevation_low);
	}
	if (edge->to->y > 0UL) {
		queue_push(edge, edge->to-width, elevation_low);
	}
	if (edge->to->x < width-1UL) {
		queue_push(edge, edge->to+1UL, elevation_low);
	}
	if (edge->to->y < height-1UL) {
		queue_push(edge, edge->to+width, elevation_low);
	}
}

void queue_push(edge_t *from, cell_t *to, int elevation_low) {
	if (to->elevation >= elevation_low && !to->visited) {
		to->visited = 1;
		set_edge(queue+queue_size, from, to);
		queue_size++;
	}
}

void set_edge(edge_t *edge, edge_t *from, cell_t *to) {
	edge->from = from;
	edge->to = to;
}

void reset_queue(void) {
	unsigned long i;
	for (i = 0UL; i < queue_size; i++) {
		queue[i].to->visited = 0;
	}
}

void set_key_col_ancestor(cell_t *peak, edge_t *stop) {
	edge_t *edge;
	key_col = peak;
	edge = stop;
	while (edge->from) {
		if (edge->to->elevation < key_col->elevation) {
			key_col = edge->to;
		}
		edge = edge->from;
	}
	ancestor = stop->to;
}

void print_cell(const char *name, cell_t *cell) {
	printf("%s: y=%lu x=%lu elevation=%d\n", name, cell->y, cell->x, cell->elevation);
}
