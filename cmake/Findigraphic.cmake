set(name igraphic)
set(tag main)

include(FetchContent)
FetchContent_GetProperties(${name})
if (NOT ${name}_POPULATED)
	FetchContent_Declare(
		${name}
		GIT_REPOSITORY https://github.com/P-E-R-R-Y/${name}
		GIT_TAG ${tag}
	)
	FetchContent_MakeAvailable(${name})
endif()
