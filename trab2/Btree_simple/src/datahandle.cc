#include "datahandle.hh"

DataHandle::DataHandle(const char *dataFile, const char *indexFile, const char *logFile) {
    _log = new LogHandle(logFile);

    if(_log == NULL) {
        std::cerr << "[Error] DataHandle coudn't create Log. Abort.\n";
        abort();
    }

    _btree = new BTree(indexFile, _log);

    if(_btree == NULL) {
        std::cerr << "[Error] DataHandle coudn't create a B-Tree. Abort.\n";

        delete _log;
        abort();
    }

    _parser = new RegisterParser(dataFile, _log);

    if(_parser == NULL) {
        std::cerr << "[Error] DataHandle coudn't create a Register parser. Abort.\n";

        delete _btree;
        delete _log;
        abort();
    }

    if(_btree->isUpdated() == BTree::Header::REVIEW) {
        RegisterParser::Register_t &lastRegister = _parser->decodeLastModifiedRegister();
        _btree->insert(lastRegister.id, _parser->lastModifiedOffset());
        _btree->setUpdated(BTree::Header::UPDATED);
    }
}

DataHandle::~DataHandle() {
    delete _btree;
    delete _parser;
    delete _log;
}

void DataHandle::insert(RegisterParser::Register_t &reg) {
    log().hold(true);
    log() << "Execucao de operacao de INSERCAO de "
          << reg.id << ", "
          << reg.titulo << ", "
          << reg.genero << ".\n";
    log().hold(false);

    _parser->hold(true);
    _parser->openDataFile();

    _btree->_history.deleteAll(); // cleans up the _history
    offset_t offset = _btree->search(reg.id, true);

    if(offset == INVALID_OFFSET) {
        _btree->setUpdated(BTree::Header::REVIEW);
        _btree->writeHeader();

        offset = _parser->pushRegister(reg);
        _btree->insertWithoutSearch(reg.id, offset);

        _btree->setUpdated(BTree::Header::UPDATED);
        _btree->writeHeader();
    }

    _parser->hold(false);
    _parser->closeDataFile();
}

RegisterParser::Register_t DataHandle::search(int id) {
    log().hold(true);
    log() << "Execucao de operacao de PESQUISA de " << id << "\n";
    log().hold(false);

    offset_t offset = _btree->search(id);

    if(offset == INVALID_OFFSET) {
        log().hold(true);
        log() << "Chave " << id << " nao encontrada.\n";
        log().hold(false);

        RegisterParser::Register_t reg;
        reg.id = DEFAULT_KEY;

        memset(reg.genero, 0, GENDER_LENGTH);
        memset(reg.titulo, 0, TITLE_LENGTH);

        return reg;
    }

    RegisterParser::Register_t &reg = _parser->decodeRegisterAt(offset);

    log().hold(true);
    log() << "Chave " << id
          << " encontrada, offset " << offset << ", "
          << "Titulo: " << reg.titulo << ", "
          << "Genero: " << reg.genero << "\n";
    log().hold(false);

    return reg;
}

void DataHandle::remove(int id) {
    offset_t offset = _btree->remove(id);

    if(offset != INVALID_OFFSET) {
        _parser->removeRegisterAt(offset);
    }
}

void DataHandle::rebuildIndexFile() {
    log().hold(true);
    log() << "Execucao da criacao do arquivo de indice "
          << _btree->indexPath() << " "
          << "com base no arquivo de dados "
          << _parser->dataPath() << ".\n";
    log().hold(false);

    _parser->openDataFile();
    _parser->hold(true);
    _parser->reset();

    offset_t offset = _parser->readOffset();

    while(offset != INVALID_OFFSET) {
        RegisterParser::Register_t &reg = _parser->decodeNextRegister();

        if(reg.id != DEFAULT_KEY) {
            _btree->insert(reg.id, offset);
        }

        offset = _parser->readOffset();
    }

    _parser->hold(false);
    _parser->closeDataFile();
}


void DataHandle::printBTree() {
    _btree->print();
}
