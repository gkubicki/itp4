/*
** Author:  Lukas Stanek
** File:    Game.cpp
** Project: Turn Based Strategy Game
*/


#include "Game.h"
#include "../tbs.h"

using namespace std;

Game::Game(){
    mUnitHolder1.reset(new std::vector<std::shared_ptr<Unit>>);
    mUnitHolder2.reset(new std::vector<std::shared_ptr<Unit>>);
    SELECTED_STATE = false;
}

void Game::initGame() {


    mPlayers[0].reset(new Player("Player1"));
    mPlayers[1].reset(new Player("Player2"));

    mPlayers[0]->setUnits(mUnitHolder1);
    mPlayers[1]->setUnits(mUnitHolder2);

    //mPlayers[0].setColor(vec4(1.0f,1.0f,0, 1.0f));
    //mPlayers[1].setColor(vec4(1.0f,0,1.0f, 1.0f));


    cout << "linking players... " << endl;
    mPlayers[0]->setNext(mPlayers[1]);
    mPlayers[1]->setNext(mPlayers[0]);


    cout << "getting instance of UnitManager" << endl;

    mUnitManager = UnitManager::getInstance();

    cout << "loading prototypes" << endl;
    mUnitManager->loadProtoypes();

    mUnitManager->printPrototypesToCout();



    generatePlayingField();





    mRounds = 1;
    mCurrentPlayer = 1;
    mStarted = time(0);


}


void Game::generatePlayingField() {

    cout << "generating playing field" << endl;

    std::vector< std::vector< shared_ptr<Hexfield> > > field;
    int size = 24;
    float SCALING = 2;

    field.resize(size);

    for(int i = 0; i<size; ++i){
        for(int j = 0; j < size; ++j){
            shared_ptr< Hexfield > newField(new Hexfield());

            newField->mPosition[0] = j*SCALING;
            newField->mPosition[1] = i*SCALING;

            if(i%2){
                newField->mPosition[0] += 0.5*SCALING;
            }

            newField->mPositionVector = glm::vec3(newField->mPosition[0], 0.f, newField->mPosition[1]);

            field[i].push_back(newField);
//            cout << field[i][j]->mPosition[0] << "/" << field[i][j]->mPosition[1] << " ";
        }
//        cout << endl;
    }

    cout << "field initialized" << endl;

    cout << "linking hexfields" << endl;

    for(int i = 0; i<size; ++i){
        for(int j = 0; j<size; ++j){
            shared_ptr<Hexfield> hexfield = field[i][j];

            //link to left
            if(j > 0){
                hexfield->linkedTo[5] = field[i][j-1];
//                cout << "linking "
//                << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                << " to " << field[i][j-1]->printPos() << endl;
            }else{
//                cout << "not linking left" << endl;
            }

            //link to right
            if(j<size-1){
                hexfield->linkedTo[2] = field[i][j+1];
//                cout << "linking "
//                << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                << " to " << field[i][j+1]->printPos() << endl;
            }else{
//                cout << "not linking right" << endl;
            }

            //decide if the line is even oder not
            if(!i%2){
                // EVEN ROW

                //link to top-left
                if(i>0 && j > 0){
                    hexfield->linkedTo[0] = field[i-1][j-1];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i-1][j-1]->printPos() << endl;
                }else{
//                    cout << "not linking top-left" << endl;
                }

                //link to top-right
                if(i>0){
                    hexfield->linkedTo[1] = field[i-1][j];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i-1][j]->printPos() << endl;
                }else{
//                    cout << "not linking top-right" << endl;
                }

                //link to bottom-right
                if(i < size-1){
                    hexfield->linkedTo[3] = field[i+1][j];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i+1][j]->printPos() << endl;
                }else{
//                    cout << "not linking bot-right" << endl;
                }

                //link to bottom-left
                if(i < size-1 && j > 0){
                    hexfield->linkedTo[4] = field[i+1][j-1];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " <<  field[i+1][j-1]->printPos() << endl;
                }else{
//                    cout << "not linking bot left" << endl;
                }

            }else{
                //UN-EVEN ROW

                //link to top-left
                if(i>0){
                    hexfield->linkedTo[0] = field[i-1][j];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i-1][j]->printPos() << endl;
                }else{
//                    cout << "not linking top-left" << endl;
                }

                //link to top-right
                if(i>0 && j<size-1){
                    hexfield->linkedTo[1] = field[i-1][j+1];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i-1][j+1]->printPos() << endl;
                }else{
//                    cout << "not linking top-right" << endl;
                }

                //link to bottom-right
                if(i < size-1 && j < size-1){
                    hexfield->linkedTo[3] = field[i+1][j+1];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " << field[i+1][j+1]->printPos() << endl;
                }else{
//                    cout << "not linking bot-right" << endl;
                }

                //link to bottom-left
                if(i < size-1){
                    hexfield->linkedTo[4] = field[i+1][j];
//                    cout << "linking "
//                    << hexfield->mPosition[0] << "/" << hexfield->mPosition[1]
//                    << " to " <<  field[i+1][j]->printPos() << endl;
                }else{
//                    cout << "not linking bot-left" << endl;
                }
            }




//            cout << endl;
        }
    }
    cout << "linking complete" << endl;
    cout << endl;

    mFirstField = field[0][0];


    //Sets Position of Magetowers
    std::shared_ptr<Unit> tower(mUnitManager->getChild("Magierturm")->clone());
    field[4][4]->setOccupation(tower);
    tower->setCurrentHexfield(field[4][4]);

    tower.reset();

    tower = mUnitManager->getChild("Magierturm")->clone();
    field[19][19]->setOccupation(std::shared_ptr<Unit>(tower));
    tower->setCurrentHexfield(field[19][19]);


    LOG_F_TRACE(GAME_LOG_PATH, "TEST trace");
    std::shared_ptr<Unit> unit = mUnitManager->getChild("Infanterie")->clone();
    LOG_F_TRACE(GAME_LOG_PATH, "unit:", unit->getName());

    mUnitHolder1->push_back(std::shared_ptr<Unit>(unit));
    field[17][17]->setOccupation(mUnitHolder1->at(0));
    mUnitHolder1->at(0)->setCurrentHexfield(field[17][17]);

}

int Game::setupField(std::shared_ptr<mgf::Node> root, std::shared_ptr<mgf::Node> actualScene,
                     std::shared_ptr<Hexfield> hexfield){

    cout << "setting up field nodes" << endl;

    std::shared_ptr<mgf::Node> newNode = root->getChild("hex.obj")->getChild("Hex")->clone();
    actualScene->add(newNode);
    newNode->translate(hexfield->mPositionVector);
    hexfield->setEngineObjectRef(newNode);

    hexfield->mIsRendered = 1;

    /**
     * This Loop will recursively call this method in every Hexfield
     */
    for(std::shared_ptr<Hexfield> field : hexfield->linkedTo){
        if(field && !field->mIsRendered){
            setupField(root, actualScene, field);
        }

    }


    if(std::shared_ptr<Unit> unit = hexfield->getOccupation()){
        if(unit->getName() == "Magierturm"){
            std::shared_ptr<mgf::Node> unitNode = root->getChild("scene.obj")->getChild("Cube")->clone();
            unit->setUnitNode(unitNode);
            actualScene->add(unitNode);
            unitNode->translate(hexfield->mPositionVector);
            unitNode->rotate(90, glm::vec3(0.f, 1.f, 0.f));
            unitNode->scale(glm::vec3(0.3f, 3.f, 0.3f));

        }else if(unit->getName() == "Infanterie"){
            std::shared_ptr<mgf::Node> unitNode = root->getChild("scene.obj")->getChild("Cube")->clone();
            unit->setUnitNode(unitNode);
            actualScene->add(unitNode);
            unitNode->translate(hexfield->mPositionVector);
            unitNode->scale(glm::vec3(.5f, .5f, .5f));

        }

    }


}


void Game::nextTurn() {
    mRounds++;
    //TODO if player > player size reset
    mCurrentPlayer++;

    std::shared_ptr<std::vector<std::shared_ptr<Unit>>> unitHolder = mPlayers[0]->mUnits;

    for(std::shared_ptr< Unit > unit : *unitHolder){
        unit->setRemainingMovement(unit->getMovement());
        unit->setTimesDefended(0);
    }


}

int Game::unitMovementWrapper(std::shared_ptr<Unit> unit,
                              std::shared_ptr<Hexfield> destination) {
    LOG_F_TRACE(GAME_LOG_PATH, "Starting unit movement ", unit->getName(), " to ",
                destination->mPosition[1], "/" , destination->mPosition[0]);

    std::shared_ptr<Hexfield> startField = unit->getCurrentHexfield();


    std::shared_ptr<Hexfield> finishedField = unit->moveTo(destination, unit);

    LOG_F_TRACE(GAME_LOG_PATH, "translating unit to endpoint");
    if(finishedField){
        //LOG_F_TRACE(GAME_LOG_PATH, finishedField->mPositionVector);
        std::shared_ptr<mgf::Node> node = unit->getUnitNode();
        finishedField->mPositionVector;
        LOG_F_TRACE(GAME_LOG_PATH, finishedField->mPosition[1], "/", finishedField->mPosition[0]);

        unit->getUnitNode()->setTranslation(finishedField->mPositionVector);

    }

}



void Game::writeStatsToDb() {
    // TODO Write game stats to some persistent storage
}





/*
 * Setter & Getter
 */

shared_ptr<Player> Game::getPlayer(int i) {
    shared_ptr<Player> player;

    switch(i){
        case 0: player = mPlayers[0]; break;
        case 1: player = mPlayers[1]; break;
        default: player.reset(); break;
    }
    return player;
}

shared_ptr<Hexfield> Game::getFirstField() {
    return mFirstField;
}


//Game::state_t Game::do_state_default(std::shared_ptr<int> clicked) {
//    if(clicked == RIGHTCLICK){
//        return STATE_DEFAULT;
//    }
//
//    if(clicked == LEFTCLICK){
//
//    }
//
//
//    return STATE_SELECTED;
//}
//
//Game::state_t Game::do_state_selected(std::shared_ptr<int> clicked) {
//    if(clicked == RIGHTCLICK){
//        return STATE_DEFAULT;
//    }
//
//
//    return STATE_SELECTED;
//}


std::shared_ptr<Hexfield> Game::getHexAt(std::shared_ptr<Hexfield> current, float x, float y) {
    float minDist;
    float curDist;
    std::shared_ptr<Hexfield> nearest(current);

    LOG_F_TRACE(GAME_LOG_PATH, "in field: ", current->mPosition[1], "/", current->mPosition[0]);


    minDist = abs(x - current->mPosition[1])
              + abs(y - current->mPosition[0]);

    for(std::shared_ptr<Hexfield> neighbor : current->linkedTo){
        if(neighbor == NULL) continue;

        curDist = abs(x - neighbor->mPosition[1]) + abs(y - neighbor->mPosition[0]);
        if(curDist < minDist){
            minDist = curDist;
            nearest = neighbor;
        }

    }


    if(nearest == current){
        LOG_F_TRACE(GAME_LOG_PATH, "Found nearest at ", current->mPosition[1], "/", current->mPosition[0]);
        return current;
    }else{
        nearest = getHexAt(nearest, x, y);
    }

    return nearest;
}


bool Game::getSelectedState() {
    return SELECTED_STATE;
}

void Game::setSelectedState(bool selectedState) {
    SELECTED_STATE = selectedState;
}

std::shared_ptr<Unit> Game::getSelectedUnit() {
    return mselectedUnit;
}

void Game::setSelectedUnit(shared_ptr<Unit> ptr) {
    mselectedUnit = ptr;
}
