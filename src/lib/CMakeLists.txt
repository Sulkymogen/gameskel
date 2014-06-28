include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})
include_directories(${BOX2D_INCLUDE_DIR})

set(LIBGAME0_SRC
  Animation.cc
  Element.cc
  Entity.cc
  Particle.cc
  Player.cc
  Random.cc
  Resource.cc
  Score.cc
  World.cc
  WorldListener.cc
)

add_library(game0 SHARED
  ${LIBGAME0_SRC}
)

target_link_libraries(game0
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
  ${BOX2D_LIBRARIES}
)

install(
  TARGETS game0
  LIBRARY DESTINATION lib
)
