#include "Chunk.hpp"
#include <list>

std::pair<land_section_t, land_section_t>
range_xor(land_section_t a, land_section_t b)
{
	// key for second value in the pair
	// 1: enter a, 2: enter b, 4: exit a, 8: exit b
	std::vector<std::pair<int, int>> x(a.size() * 2);
	for (size_t i = 0; i < a.size(); i++)
	{
		x[i * 2] = std::make_pair(a[i].bot, 1);
		x[i * 2 + 1] = std::make_pair(a[i].top, 4);
	}
	std::vector<std::pair<int, int>> y(b.size() * 2);
	for (size_t i = 0; i < b.size(); i++)
	{
		y[i * 2] = std::make_pair(b[i].bot, 2);
		y[i * 2 + 1] = std::make_pair(b[i].top, 8);
	}
	std::list<std::pair<int, int>> z;
	std::merge(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(z));
	// NOTE: z has at least size of 4 since a and b have at least 1 element

	// if 2 elements next to each other have the same .first value
	// XOR the second element's .second value to the first and remove
	// the second element.
	auto it_1 = z.begin();
	auto it_2 = std::next(it_1, 1);
	while (it_2 != z.end())
	{
		if (it_1->first == it_2->first)
		{
			it_1->second ^= it_2->second;
			auto temp = it_2;
			it_2++;
			z.erase(temp);
			if (it_2 == z.end())
				break;
		}
		it_1++;
		it_2++;
	}

	land_section_t out_a;
	land_section_t out_b;

	// state determines if is inside or outside a or b from the XOR of
	// the following codes:
	// a outside: 1, b outside: 2, a inside: 4, b inside: 8.
	int state = 0b0011;
	for (auto p : z)
	{
		// p.second tells us if we are entering / exiting a or b or both
		switch(p.second)
		{
			// entering a
			case(0b0001):
				// if inside b
				if (state & 0b1000)
					out_b.back().top = p.first;
				else
					out_a.push_back(Column{p.first, -1});
				state ^= 0b0101;
				break;
			// entering b
			case(0b0010):
				// if inside a
				if (state & 0b0100)
					out_a.back().top = p.first;
				else
					out_b.push_back(Column{p.first, -1});
				state ^= 0b1010;
				break;
			// exiting a
			case(0b0100):
				// if inside b
				if (state & 0b1000)
					out_b.push_back(Column{p.first, -1});
				else
					out_a.back().top = p.first;
				state ^= 0b0101;
				break;
			// exiting b
			case(0b1000):
				// if inside a
				if (state & 0b0100)
					out_a.push_back(Column{p.first, -1});
				else
					out_b.back().top = p.first;
				state ^= 0b1010;
				break;
			// entering a, exiting b
			case(0b1001):
				out_a.push_back(Column{p.first, -1});
				out_b.back().top = p.first;
				state ^= 0b1111;
				break;
			// entering b, exiting a
			case(0b0110):
				out_b.push_back(Column{p.first, -1});
				out_a.back().top = p.first;
				state ^= 0b1111;
				break;
			// entering a, entering b
			case(0b0011):
				state ^= 0b1111;
				break;
			// exiting a, exiting b
			case(0b1100):
				state ^= 0b1111;
				break;
			// invalid value
			default:
				std::cout << state << " " << p.second << std::endl;
				throw std::runtime_error("invalid state");
		}
	}
	return std::make_pair(out_a, out_b);
}
