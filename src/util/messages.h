#ifndef CPOOL_MESSAGES_H
#define CPOOL_MESSAGES_H

#include <iostream>
#include <sstream>
#include <string>
#include "types.h"
#include "pack.h"

//TODO: remove trash comments

namespace c2pool::p2p
{
    class Protocol;
}

namespace c2pool::messages
{

    enum Messages
    {
        error = 9999,
        version = 0
    };

    class message
    {
    public:
        std::string command;

        message(std::string cmd)
        {
            command = cmd;
        }

        void unpack(std::string item);
        string pack();

        virtual void _unpack(std::stringstream &ss) = 0;
        virtual string _pack() = 0;
        virtual void handle(p2p::Protocol *protocol) = 0;
    };

    message *fromStr(std::string str);

    class message_error : public message
    {
    public:
        message_error(const std::string cmd = "error") : message(cmd) {}
    };

    class message_version : public message
    {
    public:
        message_version(int ver, int serv, address_type to, address_type from, long _nonce, string sub_ver, int _mode, long best_hash, const std::string cmd = "version") : message(cmd)
        {
            version = ver;
            services = serv;
            addr_to = to;
            addr_from = from;
            nonce = _nonce;
            sub_version = sub_ver;
            mode = _mode;
            best_share_hash = best_hash;
        }

        void _unpack(std::stringstream &ss) override;

        std::string _pack() override;

        void handle(p2p::Protocol *protocol) override;

        //= pack.ComposedType([
        //     ('version', pack.IntType(32)),
        int version;
        //     ('services', pack.IntType(64)),
        int services;
        //     ('addr_to', bitcoin_data.address_type),
        address_type addr_to;
        //     ('addr_from', bitcoin_data.address_type),
        address_type addr_from;
        //     ('nonce', pack.IntType(64)),
        long nonce;
        //     ('sub_version', pack.VarStrType()),
        string sub_version;
        //     ('mode', pack.IntType(32)), # always 1 for legacy compatibility
        int mode;
        //     ('best_share_hash', pack.PossiblyNoneType(0, pack.IntType(256))),
        long best_share_hash; // TODO: long long?
        // ])
    };

    class message_ping : public message
    {
    public:
        message_ping(const std::string cmd = "ping") : message(cmd) {}

        void _unpack(stringstream &ss) override;

        std::string _pack() override;

        void handle(p2p::Protocol *protocol) override;

        // message_ping = pack.ComposedType([])
        //todo Empty list
    };

    class message_addrme : public message
    {
    public:
        message_addrme(int _port, const string cmd = "addrme") : message(cmd)
        {
            port = _port;
        }

        void _unpack(stringstream &ss) override;

        string _pack() override;

        void handle(p2p::Protocol *protocol) override;

        //= pack.ComposedType([
        //    ('port', pack.IntType(16)),
        int port;
        //])
    };

    class message_getaddrs : public message
    {
    public:
        message_getaddrs(int cnt, const string cmd = "getaddr") : message(cmd)
        {
            count = cnt;
        }

        void _unpack(stringstream &ss) override;

        string _pack() override;

        void handle(p2p::Protocol *protocol) override;

        //     = pack.ComposedType([
        //     ('count', pack.IntType(32)),
        int count;
        // ])
    };

    class message_addrs : public message
    {
    public:
        message_addrs(vector<addr> _addrs, const string cmd = "addrs") : message(cmd)
        {
            addrs = _addrs;
        }

        void _unpack(stringstream &ss) override;

        string _pack() override;

        void handle(p2p::Protocol *protocol) override;

        vector<addr> addrs;

        // = pack.ComposedType([
        //     ('addrs', pack.ListType(pack.ComposedType([
        //         ('timestamp', pack.IntType(64)),
        //         ('address', bitcoin_data.address_type),
        //     ]))),
        // ])
    };

    //__________________________
    /*
    
    class message_shares : public message
    {
    public:
        message_shares(share_type shrs, const string cmd = "version") : message(cmd)
        {
            shares = shrs;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> shares;

            //TODO: override operator >> for share_type;
        }

        string pack() override
        {
            ComposedType ct;
            ct.add(shares.ToString()); //todo check toString()
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_shares(<todo>);
        }

        //     = pack.ComposedType([
        //     ('shares', pack.ListType(p2pool_data.share_type)),
        share_type shares;
        // ])
    };

    class message_sharereq : public message
    { // TODO переделать ListType.
    public:
        message_sharereq(int idd, ???ListType_Int256 hshs, int prnts, ???ListType_Int256 stps, const string cmd = "version"):message(cmd)
        {
            id = idd;
            hashes = hshs;
            parents = prnts;
            stops = stps;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> id >> hashes >> parents >> stops;

            //TODO: override operator >> for ListType_Int256;
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(id);
            ct.add(hashes.ToString()); //todo check ListType_Int256, hashes.ToString()
            ct.add(parents);
            ct.add(stops.ToString()); //todo ListType_Int256, stops.ToString()
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_sharereq(<todo>);
        }

        //     = pack.ComposedType([
        //     ('id', pack.IntType(256)),
        int id;
        //     ('hashes', pack.ListType(pack.IntType(256))),
        ListType_Int256 hashes;
        //     ('parents', pack.VarIntType()),
        int parents;
        //     ('stops', pack.ListType(pack.IntType(256))),
        ListType_Int256 stops;
        // ])
    };

    class message_sharereply : public message
    { // TODO Enum и ListTypeShareType
    public:
        message_sharereply(int idd, enum rslt, ListTypeShareType shrs, const string cmd = "version") : message(cmd)
        {
            id = idd;
            result = rslt;
            shares = shrs;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> id >> result >> shares;

            //TODO: override operator >> for ListTypeShareType & enum;
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(id);
            ct.add(result); //todo check enum
            ct.add(shares); // todo check ListTypeShareType
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_sharereply(<todo>);
        }

        //     = pack.ComposedType([
        //     ('id', pack.IntType(256)),
        int id;
        //     ('result', pack.EnumType(pack.VarIntType(), {0: 'good', 1: 'too long', 2: 'unk2', 3: 'unk3', 4: 'unk4', 5: 'unk5', 6: 'unk6'})),
        enum result; // TODO enum?
        //     ('shares', pack.ListType(p2pool_data.share_type)),
        ListTypeShareType shares; // TODO ListType
        // ])
    };

    class message_bestblock : public message
    { // TODO BitcoinDataBlockHeaderType
    public:
        message_bestblock(BitcoinDataBlockHeaderType hdr, const string cmd = "version") : message(cmd)
        {
            header = hdr;

            //TODO: override operator >> for BitcoinDataBlockHeaderType;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> header;
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(header.ToString()); //todo check BitcoinDataBlockHeaderType
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_bestblock(<todo>);
        }

        //     = pack.ComposedType([
        //     ('header', bitcoin_data.block_header_type),
        BitcoinDataBlockHeaderType header;
        // ])
    };

    class message_have_tx : public message
    {
    public:
        message_have_tx(int tx_hshs, const string cmd = "version") : message(cmd)
        {
            tx_hashes = tx_hshs;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> tx_hashes;
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(tx_hashes);
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_have_tx(<todo>);
        }

        //     = pack.ComposedType([
        //     ('tx_hashes', pack.ListType(pack.IntType(256))),
        int tx_hashes;
        // ])
    };

    class message_losing_tx : public message
    { // TODO ListTypeInt256
    public:
        message_losing_tx(ListTypeInt256 tx_hshs, const string cmd = "version") : message(cmd)
        {
            tx_hashes = tx_hshs;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> tx_hashes;

            //todo override operator >> for ListTypeInt256
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(tx_hashes); //todo check ListTypeInt256
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_losing_tx(<todo>);
        }

        //     = pack.ComposedType([
        //     ('tx_hashes', pack.ListType(pack.IntType(256))),
        ListTypeInt256 tx_hashes;
        // ])
    };

    class message_remember_tx : message
    { // TODO ListTypeInt256, ListTypeTX
    public:
        message_remember_tx(ListTypeInt256 tx_hshs, ListTypeTX txss, const string cmd = "version") : message(cmd)
        {
            tx_hashes = tx_hshs;
            txs = txss;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> tx_hashes >> txs;

            //todo override operator >> forListTypeInt256, ListTypeTX
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(tx_hashes); //todo ListTypeInt256
            ct.add(txs);       //todo ListTypeTX
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_remember_tx(<todo>);
        }

        //     = pack.ComposedType([
        //     ('tx_hashes', pack.ListType(pack.IntType(256))),
        ListTypeInt256 tx_hashes;
        //     ('txs', pack.ListType(bitcoin_data.tx_type)),
        ListTypeTX txs;
        // ])
    };

    class message_forget_tx : message
    { // TODO ListTypeInt256
    public:
        message_forget_tx(ListTypeInt256 tx_hshs, const string cmd = "version") : message(cmd)
        {
            tx_hashes = tx_hshs;
        }

        void _unpack(stringstream &ss) override
        {
            ss >> tx_hashes;
            //todo override operator >> for ListTypeInt256
        }

        string _pack() override
        {
            ComposedType ct;
            ct.add(tx_hashes); //todo ListTypeInt256
            return ct.read();
        }

        void handle(p2p::Protocol *protocol)
        {
            protocol->handle_forget_tx(<todo>);
        }

        //     = pack.ComposedType([
        //     ('tx_hashes', pack.ListType(pack.IntType(256))),
        ListTypeInt256 tx_hashes;
        // ])
    }



    */

    //__________________________

} // namespace c2pool::messages

#endif //CPOOL_MESSAGES_H
